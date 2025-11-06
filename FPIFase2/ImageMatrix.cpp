#include "ImageMatrix.hpp"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <cstring>
#include "Histogram.hpp"

bool IsGrey(cv::Mat img) {
    int rows=img.rows, columns=img.cols;
    cv::Vec3b pixelBuffer;
    for (int i=0;i<rows;i++) {
        for (int j=0;j<rows;j++) {
            pixelBuffer = img.at<cv::Vec3b>(i,j);
            if (pixelBuffer[0] != pixelBuffer[1]) {return false;}
            if (pixelBuffer[1] != pixelBuffer[2]) {return false;}
        }
    }
    return true;
}

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

cv::Mat Negative(cv::Mat img) {
    cv::Mat newImg = cv::Mat::zeros(img.size(), img.type());
    cv::Vec3b pixelBuffer;
    int i, j;
    int rows=img.rows, columns=img.cols;

    for (i=0;i<rows;i++) {
        for (j=0;j<columns;j++) {
            pixelBuffer = img.at<cv::Vec3b>(i,j);
            pixelBuffer[0] = 255 - pixelBuffer[0];
            pixelBuffer[1] = 255 - pixelBuffer[1];
            pixelBuffer[2] = 255 - pixelBuffer[2];
            newImg.at<cv::Vec3b>(i,j) = pixelBuffer;
        }
    }

    return newImg;
}

cv::Mat Enlarge(cv::Mat img) {
    cv::Vec3b pixelBuffer, neighboor1, neighboor2;
    int i, j, k;
    int colorBuffer;
    int origRows=img.rows, origColumns=img.cols;
    int newRows, newColumns;

    newRows = origRows * 2 - 1;
    newColumns = origColumns * 2 - 1;
    cv::Mat newImg = cv::Mat::zeros(newRows, newColumns, img.type());
    

    for (i=0;i<newRows;i+=2) {
        for (j=0;j<newColumns;j+=2) {
            newImg.at<cv::Vec3b>(i,j) = img.at<cv::Vec3b>(i/2,j/2);
        }
    }

    for (i=0;i<newRows;i+=2) {
        for (j=1;j<newColumns;j+=2) {
            neighboor1 = newImg.at<cv::Vec3b>(i,j-1);
            neighboor2 = newImg.at<cv::Vec3b>(i,j+1);

            for (k=0;k<3;k++) {
                colorBuffer = (neighboor1[k] + neighboor2[k])/2;
                pixelBuffer[k] = static_cast<unsigned char>(colorBuffer);
            }

            newImg.at<cv::Vec3b>(i,j) = pixelBuffer;
        }
    }

    for (i=1;i<newRows;i+=2) {
        for (j=0;j<newColumns;j++) {
            neighboor1 = newImg.at<cv::Vec3b>(i-1,j);
            neighboor2 = newImg.at<cv::Vec3b>(i+1,j);

            for (k=0;k<3;k++) {
                colorBuffer = (neighboor1[k] + neighboor2[k])/2;
                pixelBuffer[k] = static_cast<unsigned char>(colorBuffer);
            }

            newImg.at<cv::Vec3b>(i,j) = pixelBuffer;
        }
    }

    return newImg;
}


cv::Mat Reduce(cv::Mat img, int sx, int sy) {
    cv::Vec3b imgFragment[sx][sy], pixelBuffer;
    int i, j, k, m, n, p, o;
    int colorBuffer;
    int origRows=img.rows, origColumns=img.cols;
    int newRows, newColumns;

    newRows = (origRows + sx -1) / sx;
    newColumns = (origColumns + sy -1) / sy;
    cv::Mat newImg = cv::Mat::zeros(newRows, newColumns, img.type());

    for (i=0;i<newRows;i++) {
        for (j=0;j<newColumns;j++) {

            m = 0;
            while ((i*sx+m < origRows) && (m < sx)) {
                n = 0;
                while ((j*sy+n <= origColumns) && (n <= sy)) {
                    imgFragment[m][n] = img.at<cv::Vec3b>(i*sx+m,j*sy+n);
                    n++;
                }
                m++;
            }
 
            for (k=0;k<3;k++) {
                colorBuffer = 0;
                for (p=0;p<m;p++) {
                    for (o=0;o<n;o++) {
                        colorBuffer += imgFragment[p][o][k];
                    }
                }

                pixelBuffer[k] = static_cast<unsigned char>(colorBuffer/(m*n));
            }

            newImg.at<cv::Vec3b>(i,j) = pixelBuffer;
        }
    }

    return newImg;
}


cv::Mat Rotate90(cv::Mat img) {
    int i, j;
    int origRows=img.rows, origColumns=img.cols;
    int newRows, newColumns;

    newRows = origColumns;
    newColumns = origRows;
    cv::Mat newImg = cv::Mat::zeros(newRows, newColumns, img.type());

    for (i=0;i<newRows;i++) {
        for (j=0;j<newColumns;j++) {
            newImg.at<cv::Vec3b>(i,j) = img.at<cv::Vec3b>(newRows-1-j,i);
        }
    }

    return newImg;
}

cv::Mat Convolution(cv::Mat img, double kernel[3][3], bool clampping) {
    cv::Mat newImg = cv::Mat::zeros(img.size(), img.type());
    cv::Vec3b imgFragment[3][3], pixelBuffer;
    double colorBuffer;
    int i, j, k, m, n;
    const int rows=img.rows, columns=img.cols;

    for (i=1;i<rows-1;i++) {
        for (j=1;j<columns-1;j++) {

            // Getting neighboor pixels
            for (m=0;m<3;m++) {
                for (n=0;n<3;n++) {
                    imgFragment[m][n] = img.at<cv::Vec3b>(i-1+m,j-1+n);
                }
            }
            
            // For each channel
            for (k=0;k<3;k++) {
                // Get new value
                colorBuffer = 0;
                for (m=0;m<3;m++) {
                    for (n=0;n<3;n++) {
                        colorBuffer += static_cast<double>(imgFragment[m][n][k] * kernel[m][n]);
                    }
                }
                // Do clampping if requested
                if (clampping) {
                    colorBuffer += 127;
                }
                // Correct value
                if (colorBuffer > 255) {
                    colorBuffer = 255;
                } else if (colorBuffer<0) {
                    colorBuffer = 0;
                }
                // Save
                pixelBuffer[k] = static_cast<unsigned char>(colorBuffer);
            }
            
            // Compose new image
            newImg.at<cv::Vec3b>(i,j) = pixelBuffer;
        }
    }

    return newImg;
}

cv::Mat Equalization(cv::Mat img) {
    cv::Mat newImg = cv::Mat::zeros(img.size(), img.type());
    cv::Vec3b oldPixel, newPixel;
    int i, j;
    int rows=img.rows, columns=img.cols;
    int totalOfPixels = rows*columns;

    std::vector<int> blue = NormalizedFreq(AcummulateFreq(Frequencies(img, 0)), totalOfPixels);
    std::vector<int> green = NormalizedFreq(AcummulateFreq(Frequencies(img, 1)), totalOfPixels);
    std::vector<int> red = NormalizedFreq(AcummulateFreq(Frequencies(img, 2)), totalOfPixels);

    for (i=0;i<rows;i++) {
        for (j=0;j<columns;j++) {
            oldPixel = img.at<cv::Vec3b>(i,j);

            newPixel[0] = blue[oldPixel[0]];
            newPixel[1] = green[oldPixel[1]];
            newPixel[2] = red[oldPixel[2]];

            newImg.at<cv::Vec3b>(i,j) = newPixel;
        }
    }

    return newImg;
}

cv::Mat Lab(cv::Mat img) {
    cv::Mat newImg = cv::Mat::zeros(img.size(), img.type());
    cv::Mat labImg;
    cv::cvtColor(img, labImg, cv::COLOR_BGR2Lab);
    cv::Vec3b pixelBuffer;
    int i, j;
    int rows=img.rows, columns=img.cols;
    int totalOfPixels = rows*columns;

    std::vector<int> frequencies = NormalizedFreq(AcummulateFreq(Frequencies(img, 0)), totalOfPixels);
    for (i=0;i<rows;i++) {
        for (j=0;j<columns;j++) {
            pixelBuffer = labImg.at<cv::Vec3b>(i,j);
            pixelBuffer[0] = frequencies[pixelBuffer[0]];
            labImg.at<cv::Vec3b>(i,j) = pixelBuffer;
        }
    }

    cv::cvtColor(labImg, newImg, cv::COLOR_Lab2BGR);
    return newImg;
}


// corrigir shadeFound para bool
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

cv::Mat Brightness(cv::Mat img, int bias) {
    cv::Mat newImg = cv::Mat::zeros(img.size(), img.type());
    cv::Vec3b pixelBuffer;
    int colorBuffer;
    int i, j, k;
    const int rows=img.rows, columns=img.cols;

    for (i=0;i<rows;i++) {
        for (j=0;j<columns;j++) {
            pixelBuffer = img.at<cv::Vec3b>(i,j);
            for (k=0;k<3;k++) {
                colorBuffer = pixelBuffer[k] + bias;
                if (colorBuffer > 255) {
                    colorBuffer = 255;
                } else if (colorBuffer<0) {
                    colorBuffer = 0;
                }
                pixelBuffer[k] = static_cast<unsigned char>(colorBuffer);
            }
            newImg.at<cv::Vec3b>(i,j) = pixelBuffer;
        }
    }

    return newImg;
}

cv::Mat Contrast(cv::Mat img, float gain) {
    cv::Mat newImg = cv::Mat::zeros(img.size(), img.type());
    cv::Vec3b pixelBuffer;
    int colorBuffer;
    int i, j, k;
    const int rows=img.rows, columns=img.cols;

    for (i=0;i<rows;i++) {
        for (j=0;j<columns;j++) {
            pixelBuffer = img.at<cv::Vec3b>(i,j);
            for (k=0;k<3;k++) {
                colorBuffer = static_cast<int>(std::floor(pixelBuffer[k] * gain));
                if (colorBuffer > 255) {
                    colorBuffer = 255;
                }
                pixelBuffer[k] = static_cast<unsigned char>(colorBuffer);
            }
            newImg.at<cv::Vec3b>(i,j) = pixelBuffer;
        }
    }

    return newImg;
}
