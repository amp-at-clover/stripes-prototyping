#ifndef __AMP_GABOR_LOCATOR__H__
#define __AMP_GABOR_LOCATOR__H__

/**
@Author: Arvind de Menezes Pereira
**/

//#define __CPP_DEBUG__

// std C++
#include <cassert>
#include <vector>
#include <cmath>
#include <string>

#ifdef __CPP_DEBUG__
#include <iostream>
#include <cstdio>
#include <sstream>
#endif

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// ZXing
#include <zxing/DecodeHints.h>
#include <zxing/oned/MultiFormatOneDReader.h>
#include <zxing/common/GreyscaleLuminanceSource.h>
#include <zxing/common/HybridBinarizer.h>
#include <zxing/BinaryBitmap.h>
#include <zxing/ReaderException.h>
#include <zxing/DecodeHints.h>

// Helper
#include "OpenCVBitmapSource.h"
#include "ampImageEnhancer.h"

// Namespaces and other stuff...
using namespace zxing;
using namespace zxing::oned;
using namespace cv;
using std::vector;
using std::cout;
using std::endl;
using std::ostringstream;

// A few pound - defines
#define ANG_RES 2                     // degree resolution
#define ANGLES_TO_CACHE 180/ANG_RES
#define RAD2DEG M_PI/180.0
#define DEG2RAD 180.0/M_PI
#define MAX_IM_PYR 4
#define RND_SEED 54321


class AmpGaborLocator 
{
// Common to all classes
	vector<cv::Mat> gabKernel; // Should be static, but link is failing!

	// Barcode decoder: Zebra-Crossing
	MultiFormatOneDReader *decoderZxing_;
    DecodeHints *decoderZxingHints_;

	// Image Enhancement
	AmpImageEnhancer aiEnh;

	// Helper random number generator
#ifdef __CPP_DEBUG__
	RNG rng;
#endif

private:
	cv::Mat bestResponse;
	vector< Mat > imgPyr;

public:
	AmpGaborLocator() : 
		gabKernel( ANGLES_TO_CACHE + 1 ), imgPyr( MAX_IM_PYR + 1 )
#ifdef __CPP_DEBUG__
		, rng( RND_SEED )
#endif
	{
		InitGaborKernel( 15, 6.0, 6.0, 0.5, 0 );
		InitDecoder();
	}

	AmpGaborLocator( int ksize, double sigma, double lambd, double gamma, double psi ) :
		gabKernel( ANGLES_TO_CACHE + 1 ), imgPyr( MAX_IM_PYR + 1 )
#ifdef __CPP_DEBUG__
		, rng( RND_SEED )
#endif

	{
		// Initialize the Gabor Kernel cache if this is the first class 
		// to ever use one.
		InitGaborKernel( ksize, sigma, lambd, gamma, psi );
		InitDecoder();
	}

	~AmpGaborLocator() {}

	// ------------- Initialize ZXing decoder ----------------------------
	void InitDecoder() {
    	decoderZxingHints_ = new DecodeHints(DecodeHints::ONED_HINT);
    	decoderZxingHints_->setTryHarder(true);
    	decoderZxing_ = new MultiFormatOneDReader(*decoderZxingHints_);
	}

	// ------------- Kernel caching methods ------------------------------
	int getKernIndexForTheta( double theta ) {
		return 	floor(theta * RAD2DEG + 0.5)/ANG_RES;
	}

	double getAngleFromIndex( int index ) {
		return DEG2RAD * ( index * ANG_RES );
	}

	cv::Mat getGaborKernel( double theta ) {
		return gabKernel[ getKernIndexForTheta(theta) ];
	}

	// int ksize=10, double sigma=3.373, double lambda=0.5, double psi=0 ( Originally )
	void InitGaborKernel( int ksize=15, double sigma=6, double lambda=6, double gamma=0.5, double psi=0 ) {
		int ktype = CV_32F;
		Size kSize = Size(ksize, ksize);

		for( int i=0; i< ANGLES_TO_CACHE; i++ ) {
			gabKernel[i] = cv::getGaborKernel( kSize, sigma, getAngleFromIndex( i ), lambda, gamma, psi, ktype );
		}
	}

	// ------------- Filter2D (simple) ----------------------
	void filter2D( cv::Mat &src, cv::Mat &dst, cv::Mat &kernel ) {
		int ddepth = -1;
		int delta  = 0;
		int borderType = BORDER_DEFAULT;
		Point anchor( -1, -1 );

		cv::filter2D( src, dst, ddepth, kernel, anchor, delta, borderType );
	}

	// ------------- Operate upon the image -----------------
	cv::Mat applyGaborAtTheta( double theta, cv::Mat &img ) {
		cv::Mat out, kern;
		kern = getGaborKernel( theta );
		this->filter2D( img, out, kern );
		return out;
	}

	void createImagePyramid( cv::Mat &img, unsigned _maxPyramidLevel ) {
		buildPyramid( img, imgPyr, _maxPyramidLevel );
	}

	// From http://stackoverflow.com/questions/2289690/opencv-how-to-rotate-iplimage
	// Rotate an image through angle degrees.
	cv::Mat rotateImage( const Mat& src, double angle ) {
		Point2f src_center(src.cols/2.0F, src.rows/2.0F);
		Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
		Mat dst;
		warpAffine(src, dst, rot_mat, src.size());
		return dst;
	}


	// ------------- Get Gabor Responses --------------------
	// Original algorithm developed by Arvind.
	double findRotationAngleFor1D_BarCodesInImage( cv::Mat &img  ) {
		double maxSum = 0;
		int maxInd = 0;
		// Get an image pyramid.
		createImagePyramid( img, MAX_IM_PYR );

		int subSamp = 3;
		cv::Mat imgToGaborify, out;
		imgPyr[subSamp].convertTo(imgToGaborify,CV_32F);

		for ( int i=0; i<ANGLES_TO_CACHE; i++ ) {
			//cv::Mat out(img.size(), CV_32F);
			this->filter2D( imgToGaborify, out, gabKernel[ i ] );
			//normalize( out, out, 100, 0, NORM_INF, -1 );
			cv::Scalar gabSum = sum( out );
#ifdef __CPP_DEBUG__
			cout<<endl<<i<<") gabSum ="<<gabSum[0]<<","<<gabSum[1]<<","<<gabSum[2]<<","<<gabSum[3]<<", maxSum="<<maxSum<<" maxInd="<<maxInd;
#endif
			if ( gabSum[0] > maxSum ) {
				maxSum = gabSum[0];
				maxInd = i;
			}
		} 
		bestResponse = img; bestResponse.convertTo( bestResponse, CV_64F);
		// Get the best response using the true image
		this->filter2D( img, bestResponse, gabKernel[ maxInd ]);
		// Normalize it...
		normalize( bestResponse, bestResponse, 200, 0, NORM_INF, -1 );
		
		// Get the angle from the max index
		return getAngleFromIndex( maxInd );
	}

	// ------------ Debug output an image ---------------------------
	void outputImage( cv::Mat &img, char *wndName ) {
#ifdef __CPP_DEBUG__
		namedWindow( wndName, WINDOW_AUTOSIZE );
		imshow( wndName, img );
		waitKey( 0 );
#endif
	}

	// ------------- Display boxes around potential barcodes -------------
	void displayRectangles( cv::Mat &img, vector< cv::Rect > rects ) {
#ifdef __CPP_DEBUG__
		cv::Mat RectDisp = img;
		for( int i=0; i< rects.size(); i++ ) {
			Scalar color = Scalar( rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255) );
		 	rectangle( RectDisp, rects[i].tl(), rects[i].br(), color, 2, 8, 0 );
		}
		outputImage( RectDisp, "Localized Barcode locations" );
#endif
	}

	// ------------- Display individual enhanced images --------------
	void dispEnhancedMiniImage( cv::Mat &mini, int i ) {
#ifdef __CPP_DEBUG__
		ostringstream sout;
		sout<<"Box "<<i;
		outputImage( mini, (char *)(sout.str().c_str()) );

#endif
	}

	// ------------ Find the most likely location of the bars --------
	// Original algorithm developed by Arvind.
	bool findBarCodeLocation( cv::Mat &img, vector<cv::Rect> &boundRect ) {
		// Assumes we have already performed a findRotationAngleFor1D_BarCode
		// and rotated the image with the best Gabor response
			
		cv::Mat imgLoG;
		// First find a LoG of this response.
		Laplacian( img, imgLoG, -1, 1, 0.5, 0, BORDER_DEFAULT ); // ddepth=-1, ksize=1, scale=0.5, delta=0 
		outputImage( imgLoG, "LaplacianOfGaussian" );

		// Second Dilate the LoG output.
		cv::Mat imgDil, element;
		cv::Point anchor( -1, -1 );
		int iterations = 10;
		int borderType = BORDER_CONSTANT;
		Scalar borderValue = morphologyDefaultBorderValue();
		dilate( imgLoG, imgDil, element, anchor, iterations, borderType, borderValue);	
		outputImage( imgDil, "DilatedImage" );

		// Third Find Rectangles in the output.
		cv::Mat threshold_output;
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		
		// Find the contours
		findContours( imgDil, contours, hierarchy, RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point( 0, 0 ) );

		// For each contour, find an approximate rectangle
		vector<vector<Point> > contours_poly( contours.size() );
		boundRect.clear();
		for( size_t i=0; i< contours.size(); i++ ) {
			approxPolyDP( Mat( contours[i] ), contours_poly[i], 3, true );
			boundRect.push_back( boundingRect( Mat( contours_poly[ i ] ) ) );
		}

		if( contours.size() ) return true;
		else return false;
		
		// Result of the rectangles found is in the vector<Rect> passed to this function.
		return false;
	}


	// Run the detection pipeline
	bool tryToDetect( cv::Mat &img, vector<string> &result, double ang ) {
		// First try to enhance the image by sharpening it and equalizing the histogram
		cv::Mat origImg = img.clone();
		//cv::equalizeHist( img, img );
		Mat img2 = aiEnh.SuperSharpen( img, 5 );
		outputImage( img2, "Sharpened Image" );

		// Take this image, find the rotation angle and compute the best Gabor response
		double likelyAngle =  -findRotationAngleFor1D_BarCodesInImage( img2 ) * RAD2DEG;
#ifdef __CPP_DEBUG__
		cout<<endl<<"Rotating by : "<<likelyAngle<<" deg."<<endl;
#endif

		// Rotate the image to that angle and find barcode locations in cv::Rect vector
		cv::Mat rotImg;
		//rotImg = rotateImage( bestResponse, likelyAngle ); 
		rotImg = rotateImage( bestResponse, likelyAngle );

		outputImage( rotImg, "Rotated Best Response" );
		vector< cv::Rect > boundRects;

		cv::Mat rotatedEnhImg = rotateImage( origImg, likelyAngle);

		if( findBarCodeLocation( rotImg, boundRects ) ) {
			#ifdef __CPP_DEBUG__
				// For debugging - output the rectangles
				displayRectangles( rotatedEnhImg, boundRects );
			#endif
			// For each cv::Rect a) Enhance local image, b) call the ZXing decoder
			for ( int i=0; i<boundRects.size(); i++ ) {
				// Create a small image from the part of the image denoted by the rectangle
				cv::Mat outMat( rotatedEnhImg, boundRects[i] );
				cv::equalizeHist( outMat, outMat );
				//outMat = aiEnh.SuperSharpen( outMat, 2 );

				dispEnhancedMiniImage(outMat,i);
				// If successful, return true. If none of the localized barcodes appear to make sense, return false
				try {
        		 Ref<OpenCVBitmapSource> source(new OpenCVBitmapSource(outMat));
        		 Ref<Binarizer> binarizer(new HybridBinarizer(source));
        		 Ref<BinaryBitmap> bitmap(new BinaryBitmap(binarizer));
        		 Ref<Result> zxingResult(decoderZxing_->decode(bitmap, *decoderZxingHints_));
        		 result.push_back( zxingResult->getText()->getText() );
				}
				catch(ReaderException e) { }
			}
		}
		return false;
	}
};

#endif
