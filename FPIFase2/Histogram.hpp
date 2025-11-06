#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <QString>

std::vector<int> Frequencies(cv::Mat img, int channel);
std::vector<int> NormalizedFreq(std::vector<int> frequencies, int maxValue);
std::vector<int> AcummulateFreq(std::vector<int> frequencies);
void DrawBarHistogram(const std::vector<int> frequencies, const QString windowName);
void DrawLineHistogram(cv::Mat img, const QString windowName);

#endif