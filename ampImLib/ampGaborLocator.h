#ifndef __AMP_GABOR_LOCATOR__H__
#define __AMP_GABOR_LOCATOR__H__

#include <cassert>
#include <vector>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using std::vector;

// A few definitions
#define ANG_RES 2                     // 2 degree resolution
#define ANGLES_TO_CACHE 180/ANG_RES
#define RAD2DEG M_PI/180.0
#define DEG2RAD 180.0/M_PI

class AmpGaborLocator
{
// Common to all classes
	static vector< Mat > gabKernel( ANGLES_TO_CACHE );
private:
	cv::Mat bestResponse;
	vector< Mat > imgPyr;
	unsigned maxPyramidLevel;

public:
	AmpGaborLocator() {
		InitGaborKernel();
		AmpGaborLocator::isInitialized = true;
	}

	AmpGaborLocator( int ksize, double sigma, double lambda, double psi ) {
		// Initialize the Gabor Kernel cache if this is the first class 
		// to ever use one.
		if( AmpGaborLocator::gabKernel.size()==0 ) {
			InitGaborKernel( int ksize, double sigma, double lambda, double psi );
		}
	}

	~AmpGaborLocator() {}


	// ------------- Kernel caching methods ---------------

	int getKernIndexForTheta( double theta ) {
		return 	floor(theta * RAD2DEG + 0.5)/ANG_RES;
	}

	double getAngleFromIndex( int index ) {
		return DEG2RAD * ( index * ANG_RES );
	}

	cv::Mat getGaborKernel( double theta ) {
		return AmpGaborLocator::gabKernel[ getKernIndexForTheta ];
	}

	// cv.getGaborKernel( 'KSize',15,'Sigma',6, ... %3.3730, ...
    // 'Theta', theta, 'Lambda',6, 'Psi', 0, 'Gamma', 0.5 );
	// ksize = 15, sigma = 6,
	// int ksize=10, double sigma=3.373, double lambda=0.5, double psi=0 // Originally.
	//
	void InitGaborKernel( int ksize=15, double sigma=6, double lambda=6, double psi=0 )
	{
		int ktype = CV_32F;
		Size kSize = Size(ksize, ksize);

		assert( theta>=0 && theta<=M_PI );

		for( int i=0; i<= ANGLES_TO_CACHE; i++ ) {
			gabKernel[ i ] = getGaborKernel( kSize, sigma, getAngleFromIndex( i ), lambda, psi, ktype );
		}
	}

	// ------------- Operate upon the image -----------------
	cv::Mat applyGaborAtTheta( double theta, cv::Mat &img ) {
		return cv.filter2D( img, getGaborKernel( theta ) );
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
		warpAffine(source, dst, rot_mat, source.size());
		return dst;
	}

	// ------------- Get Gabor Responses --------------------
	// Original algorithm developed by Arvind.
	double findRotationAngleFor1D_BarCodesInImage( cv::Mat &img  ) {
		long maxSum = 0;
		int maxInd = 0;
		// Get an image pyramid.
		CreateImagePyramid( img, 5 );

		int subSamp = 3;

		for ( int i=0; i<ANGLES_TO_CACHE; i++ ) {
			long gabSum = sum( filter2D( imgPyr[ subSamp ], gabKernel[ i ]));
			if ( gabSum > maxSum ) {
				maxSum = gabSum;
				maxInd = i;
			}
		}

		// Get the best response using the true image
		int ddepth = -1, delta = 0, borderType = BORDER_DEFAULT;
		filter2D( img, bestResponse, gabKernel[ maxInd ]);

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
	bool tryToDetect( cv::Mat &img ) {
		// Take this image, find the rotation angle and compute the best Gabor response

		// Rotate the image to that angle and find barcode locations in cv::Rect vector

		// For each cv::Rect a) Enhance local image, b) call the ZXing decoder

		// If successful, return true. If none of the localized barcodes appear to make sense, return false

	}

};


#endif
