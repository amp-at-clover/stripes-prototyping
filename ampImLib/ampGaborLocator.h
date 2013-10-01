#ifndef __AMP_GABOR_LOCATOR__H__
#define __AMP_GABOR_LOCATOR__H__

// std C++
#include <cassert>
#include <vector>
#include <cmath>
#include <string>

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

// Namespaces and other stuff...
using namespace zxing;
using namespace zxing::oned;
using namespace cv;
using std::vector;

// A few pound - defines
#define ANG_RES 2                     // 2 degree resolution
#define ANGLES_TO_CACHE 180/ANG_RES
#define RAD2DEG M_PI/180.0
#define DEG2RAD 180.0/M_PI

class AmpGaborLocator
{
// Common to all classes
	vector<cv::Mat> gabKernel; // Should be static, but link is failing!

	// Barcode decoder: Zebra-Crossing
	MultiFormatOneDReader *decoderZxing_;
    DecodeHints *decoderZxingHints_;

private:
	cv::Mat bestResponse;
	vector< Mat > imgPyr;
	unsigned maxPyramidLevel;

public:
	AmpGaborLocator() {
		if( gabKernel.size() == 0 ) {
			InitGaborKernel( 15, 6.0, 6.0, 0 );
		}
	}

	AmpGaborLocator( int ksize, double sigma, double lambda, double psi ) {
		// Initialize the Gabor Kernel cache if this is the first class 
		// to ever use one.
		if( gabKernel.size()==0 ) {
			InitGaborKernel( ksize, sigma, lambda, psi );
		}
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
	void InitGaborKernel( int ksize=15, double sigma=6, double lambda=6, double psi=0 ) {
		gabKernel.clear();
		int ktype = CV_32F;
		Size kSize = Size(ksize, ksize);

		for( int i=0; i<= ANGLES_TO_CACHE; i++ ) {
			gabKernel.push_back( cv::getGaborKernel( kSize, sigma, getAngleFromIndex( i ), lambda, psi, ktype ) );
		}
	}

	// ------------- Filter2D (simple) ----------------------
	void filter2D( cv::Mat &src, cv::Mat &dst, cv::Mat kernel ) {
		int ddepth = -1;
		int delta  = 0;
		int borderType = BORDER_DEFAULT;
		Point anchor( -1, -1 );

		cv::filter2D( src, dst, ddepth, kernel, anchor, delta, borderType );
	}

	// ------------- Operate upon the image -----------------
	cv::Mat applyGaborAtTheta( double theta, cv::Mat &img ) {
		cv::Mat out;
		this->filter2D( img, out, getGaborKernel( theta ) );
		return out;
	}

	void CreateImagePyramid( cv::Mat &img, unsigned _maxPyramidLevel ) {
		buildPyramid( img, imgPyr, maxPyramidLevel );
		maxPyramidLevel = _maxPyramidLevel;
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
		CreateImagePyramid( img, 5 );

		int subSamp = 3;

		for ( int i=0; i<ANGLES_TO_CACHE; i++ ) {
			cv::Mat out;
			this->filter2D( imgPyr[ subSamp ], out, gabKernel[ i ] );
			cv::Scalar gabSum = sum( out );
			if ( gabSum[0] > maxSum ) {
				maxSum = gabSum[0];
				maxInd = i;
			}
		}

		// Get the best response using the true image
		this->filter2D( img, bestResponse, gabKernel[ maxInd ]);

		// Get the angle from the max index
		return getAngleFromIndex( maxInd );
	}

	// ------------ Find the most likely location of the bars --------
	// Original algorithm developed by Arvind.
	bool findBarCodeLocation( cv::Mat &img, vector<cv::Rect> &boundRect ) {
		// Assumes we have already performed a findRotationAngleFor1D_BarCode
		// and rotated the image with the best Gabor response
		
		cv::Mat imgLoG;
		// First find a LoG of this response.
		Laplacian( img, imgLoG, -1, 1, 0.5, 0, BORDER_DEFAULT ); // ddepth=-1, ksize=1, scale=0.5, delta=0 

		// Second Dilate the LoG output.
		cv::Mat imgDil, element;
		cv::Point anchor( -1, -1 );
		int iterations = 8;
		int borderType = BORDER_CONSTANT;
		Scalar borderValue = morphologyDefaultBorderValue();
		dilate( imgLoG, imgDil, element, anchor, iterations, borderType, borderValue);	

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
	}

	// Run the detection pipeline
	bool tryToDetect( cv::Mat &img, vector<string> &result ) {
		// Take this image, find the rotation angle and compute the best Gabor response
		double likelyAngle =  findRotationAngleFor1D_BarCodesInImage( img );

		// Rotate the image to that angle and find barcode locations in cv::Rect vector
		cv::Mat rotImg = rotateImage( img, likelyAngle ); 
		vector< cv::Rect > boundRects;
		if( findBarCodeLocation( rotImg, boundRects ) ) {
			/*
			// For each cv::Rect a) Enhance local image, b) call the ZXing decoder
			for ( int i=0; i<boundRects.size(); i++ ) {
				// Create a small image from the part of the image denoted by the rectangle
				cv::Mat outMat( rotImg, boundRects[i] );
				// If successful, return true. If none of the localized barcodes appear to make sense, return false
        		Ref<OpenCVBitmapSource> source(new OpenCVBitmapSource(outMat));
        		Ref<Binarizer> binarizer(new HybridBinarizer(source));
        		Ref<BinaryBitmap> bitmap(new BinaryBitmap(binarizer));
        		Ref<Result> zxingResult(decoderZxing_->decode(bitmap, *decoderZxingHints_));
        		result.push_back( zxingResult->getText()->getText() );
				return true;
			}*/
		}
		return false;
	}
};

#endif
