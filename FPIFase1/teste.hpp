#ifndef TESTE_HPP
#define TESTE_HPP

#include <opencv2/opencv.hpp>

cv::Mat InvertVertically(cv::Mat img);
cv::Mat InvertHorizontally(cv::Mat img);
cv::Mat GreyScale(cv::Mat img);
cv::Mat Quantization(cv::Mat img, int numShades);

#endif