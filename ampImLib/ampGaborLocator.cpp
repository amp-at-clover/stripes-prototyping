#include <iostream>
#include <cstdlib>
#include "ampGaborLocator.h"
#include "ampImageEnhancer.h"

// OpenCV helpers
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using std::cout;
using std::endl;

void printUsage( int argc, char *argv[] ) {
	cout<<"Usage : "<<argv[0]<<" [imgname] "<<endl;	
}


int main( int argc, char *argv[] ) {
	std::cout<<"This is a test for Arvind's Gabor-based Barcode localizer"<<std::endl;
	if(	argc == 1 || argc > 2 ) {
		printUsage( argc, argv );
		exit( EXIT_FAILURE );
	} 
	
	AmpGaborLocator agl;
	Mat src = imread( argv[ 1 ], 1 );
	Mat src_gray;

	double ang = 0;

	cvtColor( src, src_gray, COLOR_BGR2GRAY );	

	const char *source_window = "Source";
	namedWindow( source_window, WINDOW_AUTOSIZE );
	imshow( source_window, src_gray );

	// Get the results here
	vector< string > resultSet;
	if( agl.tryToDetect( src_gray, resultSet, ang ) ) {
		cout<<"Found some barcodes"<<endl;
	}
	else cout<<"Didn't find anything."<<endl;

	waitKey( 0 );

	return 0;
}
