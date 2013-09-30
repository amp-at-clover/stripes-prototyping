/**
 * @file GaborKernel.cpp
 * @brief mex interface for GaborKernel
 * @author Arvind Pereira
 * @date 2013
 */
#include "mexopencv.hpp"
#include <cmath>
using namespace std;
using namespace cv;

/**
 * Main entry called from Matlab
 * @param nlhs number of left-hand-side arguments
 * @param plhs pointers to mxArrays in the left-hand-side
 * @param nrhs number of right-hand-side arguments
 * @param prhs pointers to mxArrays in the right-hand-side
*/
void mexFunction( int nlhs, mxArray *plhs[], 
			int nrhs, const mxArray *prhs[] )
{
	// Check arguments
	if (nlhs!=1 || nrhs>5)
		mexErrMsgIdAndTxt("GaborKernel:invalidArgs","Wrong number of argumetns");

	// Argument vector
	vector<MxArray> rhs( prhs, prhs+nrhs );

	// bw, gamma, psi, lambda, theta
	double bw =1.0, gamma=0.5, psi=0.0, lambda=2, theta=0.0;

	double sigma  = lambda/(M_PI * sqrt( log(2)/2 )*( pow(2.0,bw+1)/pow(2.0,bw-1) ));
	double sigmaX = sigma;
	double sigmaY = sigma/gamma;

	// Compute the kernel size
	int kernelSize = floor( 8* (sigmaY>sigmaX)?sigmaY:sigmaX );
	if ( kernelSize%2 == 0 ) kernelSize++;

	cv::Mat kernel = Mat::zeros( kernelSize, kernelSize, CV_32F );
	// Perform Meshgrid
	vector<float> x; for( float _x = -floor( kernelSize/2 ), i=0 ; _x < floor( kernelSize/2 ); _x++, i++ ) x[i]=_x; 
	vector<float> y; for( float _y = floor( kernelSize/2 ), i=0; _y >= floor( kernelSize/2 ); _y--, i++ )  y[i]=_y; 


	// Convert MxArray to cv::Mat
	cv::Mat mat = MxArray( prhs[0]).toMat();

	// Convert cv::Mat back to mxArray*
	plhs[0] = MxArray( mat );
}
