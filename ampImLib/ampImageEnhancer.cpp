/*
 * Image enhancement (Super-sharpening) to try to improve upon the images being grabbed
 * by the
 * @author  Arvind Pereira
 * @created 2013-09-24
 * @summary This is the implementation file for the localization algorithm we will
 *          try to use for barcode localization if it appears to fail with other
 *          techniques.
 */
#include "ampImageEnhancer.h"

using namespace cv;


AmpImageEnhancer::AmpImageEnhancer()
{
}

AmpImageEnhancer::~AmpImageEnhancer()
{
}


cv::Mat AmpImageEnhancer::SuperFilter( cv::Mat &imgToFilter )
{
    // Parameter tuning
    //
    Mat out = imgToFilter;

    return out;
}

cv::Mat AmpImageEnhancer::SuperSharpen( cv::Mat &imgToSharpen , double scale = 6 )
{
    // Parameter tuning
    // TODO: Hardcoded for now. Worthwhile using profiles with parameters tuned for various settings
    // eg. low-light with LED, good light with LED, good light without LED etc.
    int ddepth = -1;
    int ksize = 1;
    double delta=0;
    int borderType=BORDER_DEFAULT;

    Mat img;
    Laplacian( imgToSharpen, img, ddepth, ksize, scale, delta, borderType );
    // Now we subtract this from the original image
    Mat out = imgToSharpen - img;

    return out;
}
