#include "ImageEditingManager.hpp"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QPixmap>
#include <QLabel>
#include "ImageMatrix.hpp"
#include "Histogram.hpp"

#define DESCRIPTION_HEIGHT 20
#define SPACE 5
#define TITLE_ABOVE     DESCRIPTION_HEIGHT+SPACE*2

// Init:
ImageEditingManager::ImageEditingManager(cv::Mat newImg):
    currentImg(newImg), parameterBuffer(newImg), resetBuffer(newImg), grey(IsGrey(newImg)) {}

// Get, set, others:
void ImageEditingManager::ShowImage() {
    const uchar *qImageBuffer = (const uchar*)currentImg.data;
    // Create QImage with same dimensions as input
    QImage qImg(qImageBuffer, currentImg.cols, currentImg.rows, currentImg.step, QImage::Format_RGB888);
    imgLabel->setPixmap(QPixmap::fromImage(qImg.rgbSwapped()));
}

void ImageEditingManager::UpdateParameters() {
    currentImg = parameterBuffer.clone();
    if (quantized) {
        currentImg = Quantization(currentImg, lastQuantity);
    }
    if (contrast) {
        currentImg = Contrast(currentImg, lastContrast);
    }
    if (bright) {
        currentImg = Brightness(currentImg, lastBrightness);
    }
}

void ImageEditingManager::SetTitleLabel(QLabel *newLabel) {
    titleLabel = newLabel;
}

void ImageEditingManager::SetImgLabel(QLabel *newLabel) {
    imgLabel = newLabel;
    minHeight = window->height() - (TITLE_ABOVE+SPACE) - imgLabel->height();
}

void ImageEditingManager::SetWindow(QWidget *newWindow) {
    window = newWindow;
}

void ImageEditingManager::SetMinHeight(int newHeight) {
    minHeight = newHeight;
}

bool ImageEditingManager::GetGreyFlag() {
    return grey;
}

void ImageEditingManager::Resize() {
    int newWidth = window->width()-imgLabel->width()+parameterBuffer.cols;
    int newHeight = minHeight;
    if (newHeight < parameterBuffer.rows+TITLE_ABOVE+SPACE) {
        newHeight = parameterBuffer.rows+TITLE_ABOVE+SPACE;
    }

    window->setFixedSize(newWidth, newHeight);
    titleLabel->setFixedSize(parameterBuffer.cols, titleLabel->height());
    imgLabel->setFixedSize(parameterBuffer.cols, parameterBuffer.rows);

}

void ImageEditingManager::Save() {
    cv::imwrite("DuckyShop.jpeg", currentImg);
}

// Image operations:
void ImageEditingManager::MirrorHorizontally() {
    parameterBuffer = InvertHorizontally(parameterBuffer);
    UpdateParameters();
    ShowImage();
}

void ImageEditingManager::MirrorVertically() {
    parameterBuffer = InvertVertically(parameterBuffer);
    UpdateParameters();
    ShowImage();
}

void ImageEditingManager::ConvertGreyscale() {
    if (!grey) {
        parameterBuffer = GreyScale(parameterBuffer);
        grey = true;
        UpdateParameters();
        ShowImage();
    }
}

void ImageEditingManager::ConvertNegative() {
    parameterBuffer = Negative(parameterBuffer);
    UpdateParameters();
    ShowImage();
}

void ImageEditingManager::ZoomIn() {
    parameterBuffer = Enlarge(parameterBuffer);
    Resize();
    UpdateParameters();
    ShowImage();
}

void ImageEditingManager::ZoomOut(int sx, int sy) {
    parameterBuffer = Reduce(parameterBuffer, sx, sy);
    Resize();
    UpdateParameters();
    ShowImage();
}

void ImageEditingManager::Rotate() {
    parameterBuffer = Rotate90(parameterBuffer);
    Resize();
    UpdateParameters();
    ShowImage();
}

// Image filters:
bool IsLowPass(double kernel[3][3]) {
    int i, j;
    double sum = 0;
    bool lowPass = true;

    for (i=0;i<3;i++) {
        for (j=0;j<3;j++) {
            if (kernel[i][j] < 0) {
                return false;
            }
            sum += kernel[i][j];
        }
    }

    if (sum == 1) {
        return true;
    }
    else return false;
}

void ImageEditingManager::ApplyFilter(double kernel[3][3], bool clampping) {
    int i, j;
    double invertedKernel[3][3];

    for (i=0;i<3;i++) {
        for (j=0;j<3;j++) {
            invertedKernel[i][j] = kernel[2-i][2-j]; 
        }
    }

    // Apply greyscale if not low-pass
    if (!IsLowPass(kernel) && !grey) {
        parameterBuffer = GreyScale(parameterBuffer);
        grey = true;
    }

    /* In this case, I'm not so sure that updating parameters as quantization after 
    convolution would not make a difference if compared to updating parameters before.*/

    parameterBuffer = Convolution(parameterBuffer, invertedKernel, clampping);
    UpdateParameters();
    ShowImage();
}


// Histogram functions:
void ImageEditingManager::ShowHistogram() {
    if (!grey) {
        parameterBuffer = GreyScale(parameterBuffer);
        grey = true;
        UpdateParameters();
        ShowImage();
    }
    DrawBarHistogram(Frequencies(currentImg, 0), "Current histogram");
}

void ImageEditingManager::EqualizeImgHistogram() {
    if (grey) {
        DrawBarHistogram(Frequencies(currentImg, 0), "Previous frequencies");
    } else {
        DrawLineHistogram(currentImg, "Previous frequencies");
    }
   
    parameterBuffer = Equalization(parameterBuffer);
    UpdateParameters();
    ShowImage();

    if (grey) {
        DrawBarHistogram(Frequencies(currentImg, 0), "Equalized frequencies");
    } else {
        DrawLineHistogram(currentImg, "Equalized frequencies");
    }
}

void ImageEditingManager::EqualizeTroughLAB() {
    DrawLineHistogram(currentImg, "Previous frequencies");
   
    parameterBuffer = Lab(parameterBuffer);
    UpdateParameters();
    ShowImage();

    DrawLineHistogram(currentImg, "Equalized frequencies");
}


// Image parameters:
void ImageEditingManager::AdjustQuantization(int numShades) {
    lastQuantity = numShades;
    quantized = true;
    if (!grey) {
        parameterBuffer = GreyScale(parameterBuffer);
        grey = true;
    }
    UpdateParameters();
    ShowImage();
}

void ImageEditingManager::AdjustBrightness(int bias) {
    lastBrightness = bias;
    bright = true;
    UpdateParameters();
    ShowImage();
}

void ImageEditingManager::AdjustContrast(float gain) {
    lastContrast = gain;
    contrast = true;
    UpdateParameters();
    ShowImage();
}

// Reset and save
void ImageEditingManager::Reset() {
    parameterBuffer = resetBuffer.clone();
    currentImg = resetBuffer.clone();
    Resize();

    quantized = false;
    grey = false;
    bright = false;
    contrast = false;
    ShowImage();
}