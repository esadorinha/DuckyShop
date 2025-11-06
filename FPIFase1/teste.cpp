
#include "teste.hpp"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <cstring> // Para memcpy

cv::Mat InvertVertically(cv::Mat img) {
    cv::Mat newImg = cv::Mat::zeros(img.size(), img.type());
    int i;
    int rows = img.rows;
    int rowSize = img.step[0];

    for (i=0;i<rows;i++) {
        memcpy(newImg.ptr(i), img.ptr(rows-i-1), rowSize);
    }

    return newImg;
}

cv::Mat InvertHorizontally(cv::Mat img) {
    cv::Mat newImg = cv::Mat::zeros(img.size(), img.type());
    cv::Mat columnBuffer;
    int i;
    int columns = img.cols;


    for(i=0;i<columns;i++) {
        columnBuffer = img.col(i);
        columnBuffer.copyTo(newImg.col(columns-i-1));
    }

    return newImg;
}

cv::Mat GreyScale(cv::Mat img) {
    cv::Mat newImg = cv::Mat::zeros(img.size(), img.type());
    cv::Vec3b pixelBuffer;
    uchar lumBuffer;
    int i, j;
    int rows=img.rows, columns=img.cols;

    for (i=0;i<rows;i++) {
        for (j=0;j<columns;j++) {
            pixelBuffer = img.at<cv::Vec3b>(i,j);
            lumBuffer = uchar(0.114*pixelBuffer[0] + 0.587*pixelBuffer[1] + 0.299*pixelBuffer[2]);
            pixelBuffer[0] = lumBuffer;
            pixelBuffer[1] = pixelBuffer[0];
            pixelBuffer[2] = pixelBuffer[1];
            newImg.at<cv::Vec3b>(i,j) = pixelBuffer;
        }
    }

    return newImg;
}

cv::Mat Quantization(cv::Mat img, int numShades) {
    cv::Mat newImg = cv::Mat::zeros(img.size(), img.type());
    cv::Vec3b pixelBuffer;
    uchar lumBuffer;
    int i, j, shadeFound;
    int rows=img.rows, columns=img.cols;
    int minShade=255, maxShade=0;
    float tBin, base;

    for (i=0;i<rows;i++) {
        for (j=0;j<columns;j++) {
            lumBuffer = img.at<cv::Vec3b>(i,j)[0];
            if (lumBuffer < minShade) {
                minShade = lumBuffer;
            } else if (lumBuffer > maxShade) {
                maxShade = lumBuffer;
            }
        }
    }

    if (numShades >= maxShade-minShade+1) {
        return img;
    } else {
        tBin = ((float)maxShade-(float)minShade+1.0)/numShades;
        for (i=0;i<rows;i++) {
            for (j=0;j<columns;j++) {
                lumBuffer = img.at<cv::Vec3b>(i,j)[0];
                base = minShade-0.5+tBin;
                shadeFound = 0;
                while (base < 255 && shadeFound == 0) {
                    if (lumBuffer < base) {
                        base -= tBin/2;
                        lumBuffer = (int)base;
                        shadeFound = 1;
                    } else {
                        base += tBin;
                    }
                }
                if (shadeFound == 0) {lumBuffer = (int)(base-tBin/2.0);}

                pixelBuffer[0] = lumBuffer;
                pixelBuffer[1] = pixelBuffer[0];
                pixelBuffer[2] = pixelBuffer[1];
                newImg.at<cv::Vec3b>(i,j) = pixelBuffer;
            }
        }
    }

    return newImg;
}