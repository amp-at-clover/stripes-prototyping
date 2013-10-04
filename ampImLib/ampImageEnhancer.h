#ifndef __AMP_IMAGE_ENHANCER_H__
#define __AMP_IMAGE_ENHANCER_H__

#include <string.h>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class AmpImageEnhancer
{


public:
    AmpImageEnhancer();
    ~AmpImageEnhancer();
    cv::Mat SuperSharpen( cv::Mat &imgToSharpen, double );
    cv::Mat SuperFilter ( cv::Mat &imgToFilter );
    void UnsharpMaskFilter( cv::Mat &src, cv::Mat &dst );
};

#endif // __AMP_IMAGE_ENHANCER_H__
