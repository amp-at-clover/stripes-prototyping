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
	//std::cout<<"This is a test for Arvind's Gabor-based Barcode localizer"<<std::endl;
	if(	argc == 1 || argc > 2 ) {
		printUsage( argc, argv );
		exit( EXIT_FAILURE );
	} 
	
	AmpGaborLocator agl;
	Mat src = imread( argv[ 1 ], 1 );
	Mat src_gray;

	double ang = 0;

	cvtColor( src, src_gray, COLOR_BGR2GRAY );	

	// Get the results here
	vector< string > resultSet;
	agl.tryToDetect( src_gray, resultSet, ang );
	if( resultSet.size() ) {
		cout<<"Found some barcodes:"<<endl;
		for( int i=0; i < resultSet.size(); i++ ) {
			cout<<resultSet[i]<<endl;
			/*
			char *wndName="Window";
			namedWindow( wndName, WINDOW_AUTOSIZE );
			imshow( wndName, src );
			waitKey( 0 );
			*/
		}
	}
	else cout<<"None"<<endl;

	//agl.outputImage( src, "Source Image" );

	return 0;
}
