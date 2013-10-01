#ifndef __OPENCV_BITMAPSOURCE_H__
#define __OPENCV_BITMAPSOURCE_H__

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

using namespace cv;
using namespace zxing;
using namespace zxing::oned;


class OpenCVBitmapSource : public LuminanceSource
{
private:
    cv::Mat m_pImage;
public:
    OpenCVBitmapSource(Mat &image);
    int getWidth() const;
    int getHeight() const;
    ArrayRef<char> getRow(int y, ArrayRef<char> row) const;
    ArrayRef<char> getMatrix() const;
    bool isCropSupported();
    Ref<LuminanceSource> crop(int left, int top, int width, int height);
    bool isRotateSupported();
    Ref<LuminanceSource> rotateCounterClockwise();
};


// ZXing luminance source powered by OpenCV Mat
OpenCVBitmapSource::OpenCVBitmapSource(Mat &image) : LuminanceSource(image.cols, image.rows)
{
            m_pImage = image;//.clone();
};
int OpenCVBitmapSource::getWidth() const { return m_pImage.cols; }
int OpenCVBitmapSource::getHeight() const { return m_pImage.rows; }
ArrayRef<char> OpenCVBitmapSource::getRow(int y, ArrayRef<char> row) const
{
    int width_ = getWidth();
    if (!row)
        row = ArrayRef<char>(width_);
    const char *p = m_pImage.ptr<char>(y);
    for(int x = 0; x<width_; ++x, ++p)
        row[x] = *p; 
    return row;
}
ArrayRef<char> OpenCVBitmapSource::getMatrix() const
{
    int width_ = getWidth();
    int height_ =  getHeight();
    ArrayRef<char> matrix = ArrayRef<char>(width_*height_);
    for (int y = 0; y < height_; ++y)
    {   
        const char *p = m_pImage.ptr<char>(y);
        for(int x = 0; x < width_; ++x, ++p)
        {   
            matrix[y*width_ + x] = *p; 
        }
    }
    return matrix;
}

bool OpenCVBitmapSource::isCropSupported() { return false; }
Ref<LuminanceSource> OpenCVBitmapSource::crop(int left, int top, int width, int height) {}
bool OpenCVBitmapSource::isRotateSupported() { return false; }
Ref<LuminanceSource> OpenCVBitmapSource::rotateCounterClockwise() {}

#endif
