#ifndef __AMP_FAST_LOCATOR__H__
#define __AMP_FAST_LOCATOR__H__

/**
@Author : Arvind de Menezes Pereira
@Summary: Barcode location using fast masks as compared to Gabor. (and detection through Zxing).
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
#include <zxing/MultiFormatReader.h>
#include <zxing/oned/MultiFormatOneDReader.h>
#include <zxing/qrcode/QRcodeReader.h>
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
//using namespace zxing::oned;
using zxing::MultiFormatReader;
using namespace zxing::qrcode;
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
#define MAX_IM_PYR 5
#define RND_SEED 54321


class AmpFastLocator
{
// Common to all classes
	vector<cv::Mat> gabKernel; // Should be static, but link is failing!

	// Barcode decoder: Zebra-Crossing
	//MultiFormatOneDReader *decoderZxing_;
	MultiFormatReader *decoderZxing_;
	//QRCodeReader *decoderZxing_;
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
	AmpFastLocator() : imgPyr( MAX_IM_PYR + 1 )
#ifdef __CPP_DEBUG__
		, rng( RND_SEED )
#endif
	{
		InitDecoder();
	}

	AmpFastLocator( int ksize, double sigma, double lambd, double gamma, double psi ) :
		imgPyr( MAX_IM_PYR + 1 )
#ifdef __CPP_DEBUG__
		, rng( RND_SEED )
#endif

	{
		InitDecoder();
	}

	~AmpFastLocator() {}

	// ------------- Initialize ZXing decoder ----------------------------
	void InitDecoder() {
    	decoderZxingHints_ = new DecodeHints(DecodeHints::ONED_HINT | DecodeHints::QR_CODE_HINT );
    	//decoderZxingHints_ = new DecodeHints( BarcodeFormat::QR_CODE );
    	decoderZxingHints_->setTryHarder(true);
    	//decoderZxing_ = new MultiFormatOneDReader(*decoderZxingHints_);
    	decoderZxing_ = new MultiFormatReader();
		//decoderZxing_ = new QRCodeReader();	
		decoderZxing_->setHints( *decoderZxingHints_ );
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
	void applyGaborAtTheta( cv::Mat &src, cv::Mat &dst, double theta ) {
		cv::Mat kern; int ksize=15; int ktype = CV_32F; Size kSize = Size(ksize, ksize);
		double sigma=6, lambda=3, gamma=10, psi=0; // Gabor parameters

		kern = cv::getGaborKernel( kSize, sigma, theta, lambda, gamma, psi, ktype );
		this->filter2D( src, dst, kern );
	}

	void createImagePyramid( cv::Mat &img, unsigned _maxPyramidLevel ) {
		buildPyramid( img, imgPyr, _maxPyramidLevel );
	}

	void binarizeImage( cv::Mat &src, cv::Mat &dst, double threshVal=0.5, double maxVal=1.0, int thresholdType=THRESH_TRUNC ) {
		//int thresholdType = THRESH_TRUNC; // cv::THRESH_BINARY;
		threshold( src, dst, threshVal, maxVal, thresholdType );
	}

	// From http://stackoverflow.com/questions/2289690/opencv-how-to-rotate-iplimage
	// Rotate an image through angle degrees.
	void rotateImage( Mat& src, Mat &dst, double angle ) {
		Point2f src_center(src.cols/2.0F, src.rows/2.0F);
		Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
		warpAffine(src, dst, rot_mat, src.size());
	}

	// ------------- Get Sobel responses --------------------
	// Original algorithm developed by Arvind.
	double findRotationAngleFor1D_BarCodesInImage( cv::Mat &img  ) {
		double maxSum = 0;
		int maxInd = 0;
		// Get an image pyramid.
		createImagePyramid( img, MAX_IM_PYR );

		int subSamp = 4;
		cv::Mat out;
		cv::Mat SobMask = (Mat_<float>(3,3) << -1, 0, 1,
											-2, 0, 2,
											-1, 0, 1 );

		const double sobThresh = 30.0;
		this->filter2D( imgPyr[subSamp], out, SobMask );
		this->binarizeImage( out, out, sobThresh, 255, THRESH_TRUNC );
		cv::Scalar sobSum = sum( out );
		maxSum = sobSum[0];
		for ( int i=1; i<ANGLES_TO_CACHE; i++ ) {
			cv::Mat rotatedImg;
			rotateImage( imgPyr[subSamp], rotatedImg, i * ANG_RES );
			this->filter2D( rotatedImg, out, SobMask );
			this->binarizeImage( out, out, sobThresh, 255, THRESH_TRUNC );
			//normalize( out, out, 100, 0, NORM_INF, -1 );
			sobSum = sum( out );
#ifdef __CPP_DEBUG__
			cout<<endl<<i<<") sobSum ="<<sobSum[0]<<", maxSum="<<maxSum<<" maxInd="<<maxInd;
#endif
			if ( sobSum[0] > maxSum ) {
				maxSum = sobSum[0];
				maxInd = i;
			}
		} 
		// Now rotate the image through the best response we got.
		double angleToRotateBy = maxInd * ANG_RES + 90.0;
		this->rotateImage( img, bestResponse, angleToRotateBy );
		bestResponse.convertTo( bestResponse, CV_32F);
		// Get the best response using the true image
		this->applyGaborAtTheta( bestResponse, bestResponse, 0 ); // image rotated already. Use zero deg for Gabor.
		// Normalize it...
		normalize( bestResponse, bestResponse, 255, 0, NORM_INF, -1 );
		
		return angleToRotateBy;
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
		int iterations = 4;
		int borderType = BORDER_CONSTANT;
		Scalar borderValue = morphologyDefaultBorderValue();
		dilate( imgLoG, imgDil, element, anchor, iterations, borderType, borderValue);
		imgDil.convertTo(imgDil, CV_8U);
		this->binarizeImage( imgDil, imgDil, 25, 255, THRESH_BINARY );
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

	void filterRectanglesBySize( const vector< cv::Rect > &rects, vector< cv::Rect > &filteredRects, int minWidth, int minHeight ) {
		filteredRects.clear();
		for( int i=0; i<rects.size(); i++ ) {
			if( rects[i].width > minWidth && rects[i].height > minHeight ) {
				filteredRects.push_back( rects[i] );
			}
		}
	}

	// Run the detection pipeline
	bool tryToDetect( cv::Mat &img, vector<string> &result ) {
		// First try to enhance the image by sharpening it and equalizing the histogram
		cv::Mat origImg = img.clone();
		//cv::equalizeHist( img, img );
		//img = aiEnh.SuperSharpen( img, 10 );
		//outputImage( img2, "Sharpened Image" );

		// Take this image, find the rotation angle and compute the best Gabor response
		double likelyAngle =  findRotationAngleFor1D_BarCodesInImage( img );
#ifdef __CPP_DEBUG__
		cout<<endl<<"Rotating by : "<<likelyAngle<<" deg."<<endl;
#endif

		// Rotate the image to that angle and find barcode locations in cv::Rect vector
		cv::Mat rotImg;
		rotateImage( img, rotImg, likelyAngle );
		//rotateImage( bestResponse, rotImg, likelyAngle );

		outputImage( bestResponse, "Rotated Best Response" );
		vector< cv::Rect > boundRects, filteredRects;

		cv::Mat rotatedEnhImg;
		rotateImage( origImg, rotatedEnhImg, likelyAngle);

		if( findBarCodeLocation( rotImg, boundRects ) ) {

			filterRectanglesBySize( boundRects, filteredRects, 20, 20 ); // ignore rects smaller than 15 x 15 px

			#ifdef __CPP_DEBUG__
				// For debugging - output the rectangles
				displayRectangles( rotatedEnhImg, filteredRects );
			#endif
			// For each cv::Rect a) Enhance local image, b) call the ZXing decoder
			for ( int i=0; i< filteredRects.size(); i++ ) {
				// Create a small image from the part of the image denoted by the rectangle
				cv::Mat outMat( rotatedEnhImg, filteredRects[i] );
				//cv::equalizeHist( outMat, outMat );
				//aiEnh.UnsharpMaskFilter( outMat, outMat );
				//int enhLevel = 1;
				//outMat = aiEnh.SuperSharpen( outMat, enhLevel );
				cv::equalizeHist( outMat, outMat );
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

#endif // __AMP_FAST_LOCATOR__H__
