#ifndef IMAGEMATRIX_HPP
#define IMAGEMATRIX_HPP

#include <opencv2/opencv.hpp>

bool IsGrey(cv::Mat img);

cv::Mat InvertVertically(cv::Mat img);
cv::Mat InvertHorizontally(cv::Mat img);
cv::Mat GreyScale(cv::Mat img);
cv::Mat Negative(cv::Mat img);
cv::Mat Enlarge(cv::Mat img);
cv::Mat Reduce(cv::Mat img, int sx, int sy);
cv::Mat Rotate90(cv::Mat img);
cv::Mat Convolution(cv::Mat img, double kernel[3][3], bool clampping);
cv::Mat Equalization(cv::Mat img);
cv::Mat Lab(cv::Mat img);
cv::Mat Quantization(cv::Mat img, int numShades);
cv::Mat Brightness(cv::Mat img, int bias);
cv::Mat Contrast(cv::Mat img, float gain);

#endif