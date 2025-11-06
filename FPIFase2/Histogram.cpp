#include "Histogram.hpp"
#include <QWidget>
#include <QVBoxLayout>
#include <QString>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <vector>

#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 600
QT_CHARTS_USE_NAMESPACE


std::vector<int> Frequencies(cv::Mat img, int channel) {
    std::vector<int> frequencies(256, 0);
    int i, j;
    const int rows=img.rows, columns=img.cols;

    for (i=0;i<rows;i++) {
        for (j=0;j<columns;j++) {
            frequencies[img.at<cv::Vec3b>(i,j)[channel]]++;
        }
    }

    return frequencies;
}

std::vector<int> NormalizedFreq(std::vector<int> frequencies, int maxValue) {
    std::vector<int> normalFrequencies(frequencies.size(), 0);
    int i, j;
    double normalBuffer;

    for (i=0;i<frequencies.size();i++) {
        normalBuffer = (static_cast<double>(frequencies[i])/maxValue) * 255;
        normalFrequencies[i] = static_cast<int>(normalBuffer);
    }

    return normalFrequencies;
}

std::vector<int> AcummulateFreq(std::vector<int> frequencies) {
    std::vector<int> acumFrequencies = frequencies;
    int i, j;

    for (i=1;i<frequencies.size();i++) {
        acumFrequencies[i] += acumFrequencies[i-1];
    }

    return acumFrequencies;
}

void DrawBarHistogram(const std::vector<int> frequencies, const QString windowName) {
    
    QBarSet *set = new QBarSet("");
    for (int value : frequencies) {*set << value;}
    set->setColor(Qt::black);

    QBarSeries *series = new QBarSeries();
    series->append(set);
    series->setBarWidth(0.9);
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->hide();

    QStringList categories;
    for (int i=0;i<frequencies.size();++i) {
        categories << QString::number(frequencies[i]);
    }

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, frequencies.size()-1);
    axisX->setTickCount(10); 
    axisX->setLabelFormat("%i");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, *std::max_element(frequencies.begin(), frequencies.end()));
    axisY->setTickCount(10); 
    axisY->setLabelFormat("%i");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QWidget *window = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(window);
    layout->addWidget(chartView);
    window->setWindowTitle(windowName);
    window->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    window->show();
}

void DrawLineHistogram(cv::Mat img, const QString windowName) {
    int rows=img.rows, columns=img.cols;
    int totalOfPixels = rows*columns;

    std::vector<int> blue = Frequencies(img, 0);
    std::vector<int> green = Frequencies(img, 1);
    std::vector<int> red = Frequencies(img, 2);
    
    QLineSeries *blueLineSeries = new QLineSeries();
    QLineSeries *greenLineSeries = new QLineSeries();
    QLineSeries *redLineSeries = new QLineSeries();
    blueLineSeries->setColor(QColor(0, 0, 255));
    greenLineSeries->setColor(QColor(0, 255, 0));
    redLineSeries->setColor(QColor(255, 0, 0));
    for (size_t i = 0; i < blue.size(); ++i) {
        blueLineSeries->append(i, blue[i]);
        greenLineSeries->append(i, green[i]);
        redLineSeries->append(i, red[i]);
    }

    QChart *chart = new QChart();
    chart->addSeries(blueLineSeries);
    chart->addSeries(greenLineSeries);
    chart->addSeries(redLineSeries);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->hide();

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, blue.size()-1);
    axisX->setTickCount(10); 
    axisX->setLabelFormat("%i");
    chart->addAxis(axisX, Qt::AlignBottom);
    blueLineSeries->attachAxis(axisX);
    greenLineSeries->attachAxis(axisX);
    redLineSeries->attachAxis(axisX);

    int maxFrequency = std::max(*std::max_element(blue.begin(), blue.end()),
                                std::max(*std::max_element(green.begin(), green.end()),
                                        *std::max_element(red.begin(), red.end())));

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxFrequency);
    axisY->setTickCount(10); 
    axisY->setLabelFormat("%i");
    chart->addAxis(axisY, Qt::AlignLeft);
    blueLineSeries->attachAxis(axisY);
    greenLineSeries->attachAxis(axisY);
    redLineSeries->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QWidget *window = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(window);
    layout->addWidget(chartView);
    window->setWindowTitle(windowName);
    window->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    window->show(); 
}