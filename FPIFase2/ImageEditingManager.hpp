#ifndef IMAGEEDITINGMANAGER_HPP
#define IMAGEEDITINGMANAGER_HPP

#include <opencv2/opencv.hpp>
#include <QLabel>
#include <QWidget>

class ImageEditingManager {

private:
    cv::Mat currentImg;
    cv::Mat parameterBuffer;
    cv::Mat resetBuffer;
    int minHeight;
    QWidget *window;
    QLabel *imgLabel;
    QLabel *titleLabel;
    bool grey, 
         quantized = false, 
         bright = false,
         contrast = false;
    int lastQuantity = 0,
        lastBrightness = 0;
    float lastContrast = 0;

public:
    // Init:
    ImageEditingManager(cv::Mat newImg);

    // Get, set, others:
    void ShowImage();
    void UpdateParameters();
    void SetTitleLabel(QLabel *newLabel);
    void SetImgLabel(QLabel *newLabel);
    void SetWindow(QWidget *newWindow);
    void SetMinHeight(int newHeight);
    bool GetGreyFlag();
    void Resize();

    // Image operations:
    void MirrorHorizontally();
    void MirrorVertically();
    void ConvertGreyscale();
    void ConvertNegative();
    void ZoomIn();
    void ZoomOut(int sx, int sy);
    void Rotate();

    // Image filters:
    void ApplyFilter(double kernel[3][3], bool clampping);

    // Histogram functions:
    void ShowHistogram();
    void EqualizeImgHistogram();
    void EqualizeTroughLAB();

    // Image parameters:
    void AdjustQuantization(int numShades);
    void AdjustBrightness(int bias);
    void AdjustContrast(float gain);

    // Reset and save
    void Reset();
    void Save();
};

#endif