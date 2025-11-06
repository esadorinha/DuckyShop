#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSlider>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include "ImageEditingManager.hpp"
#include "ImageMatrix.hpp"
#include "Histogram.hpp"

#define DESCRIPTION_HEIGHT 20
#define COMMANDS_HEIGHT 280
#define COMMANDS_WIDTH 190
#define BTN_HEIGHT 20
#define BTN_WIDTH 180
#define SLIDER_HEIGHT 20
#define SLIDER_WIDTH 150
#define SLIDER_NUM_WIDTH 28
#define SLIDER_TITLE_HEIGHT 15
#define SPACE 5

#define IMG_AREA_START  COMMANDS_WIDTH
#define TITLE_ABOVE     DESCRIPTION_HEIGHT+SPACE*2
#define BTN_ABOVE       25 //BTN_HEIGHT+SPACE
#define SLIDER_ABOVE    SLIDER_TITLE_HEIGHT+SLIDER_HEIGHT+SPACE*2


int main(int argc, char *argv[]) {

    // 0. GET IMAGE

    // 0.1 Check provided parameters
    if (argc < 2) {
        std::cerr << "ERROR: you must provide image path!" << std::endl;
        std::cerr << "Usage example: " << argv[0] << " <caminho_da_imagem>" << std::endl;
        return -1;
    }

    // 0.2 Open main image
    cv::Mat mainImg = cv::imread(argv[1]);
    if (mainImg.empty()) {
        std::cout << "Failed to open image!" << std::endl;
        return -1;
    }


    // 1. INICIAL SETTINGS
    
    // 1.1 Set current image (editing variable) and buffer for quantization
    cv::Mat currentImg = mainImg;
    cv::Mat quantBuffer = currentImg;
    ImageEditingManager img(mainImg);

    // 1.2 Set window inicial sizes based on chosen image
    int windowWidth = COMMANDS_WIDTH + mainImg.cols*2 + SPACE*2;
    int windowHeight = DESCRIPTION_HEIGHT + mainImg.rows + SPACE*3;


    // 2. APPLICATION AND IMAGES SETUP

    // 2.1 Inicialize Qt application
    QApplication app(argc, argv);

    // 2.2 Inicialize application window and labels
    QWidget window;
    window.setWindowTitle("Ducky Shop");
    window.setFixedSize(windowWidth, windowHeight);

    QLabel *originalImg = new QLabel(&window);
    QLabel *editingImg = new QLabel(&window);

    // 2.3 Set images position to the rigth
    originalImg->setGeometry(IMG_AREA_START, TITLE_ABOVE, mainImg.cols, mainImg.rows);
    editingImg->setGeometry(IMG_AREA_START+mainImg.cols+SPACE, TITLE_ABOVE, mainImg.cols, mainImg.rows);

    // 2.4 Show original image and set correct label for editing image
    img.SetWindow(&window);
    img.SetImgLabel(originalImg);
    img.ShowImage();
    img.SetImgLabel(editingImg);
    img.ShowImage();

    // 2.5 Prepare images descriptions
    QLabel *title1 = new QLabel("<h3>Original</h3>", &window);
    title1->setGeometry(IMG_AREA_START, SPACE, mainImg.cols, DESCRIPTION_HEIGHT);
    title1->setAlignment(Qt::AlignCenter);
    QLabel *title2 = new QLabel("<h3>Editing</h3>", &window);
    title2->setGeometry(IMG_AREA_START+mainImg.cols+SPACE, SPACE, mainImg.cols, DESCRIPTION_HEIGHT);
    title2->setAlignment(Qt::AlignCenter);
    img.SetTitleLabel(title2);


    // 3. OPERATION BUTTONS SETUP
    int currentHeight = SPACE;

    // Set font for headers
    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    font.setItalic(true);

    // 3.1 Options label
    QLabel *title3 = new QLabel("Options", &window);
    title3->setGeometry(SPACE, currentHeight, BTN_WIDTH, DESCRIPTION_HEIGHT);
    title3->setAlignment(Qt::AlignCenter);
    title3->setFont(font);
    currentHeight = TITLE_ABOVE;

    // 3.2 Button to mirror horizontally:
    QPushButton *btnMirrorH = new QPushButton("Mirror horizontally", &window);
    btnMirrorH->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnMirrorH, &QPushButton::clicked, [&img]() {
        img.MirrorHorizontally();
    });
    currentHeight += BTN_ABOVE;

    // 3.3 Button to mirror vertically:
    QPushButton *btnMirrorV = new QPushButton("Mirror vertically", &window);
    btnMirrorV->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnMirrorV, &QPushButton::clicked, [&img]() {
        img.MirrorVertically();
    });
    currentHeight += BTN_ABOVE;

    // 3.4 Button to turn to greyscale:
    QPushButton *btnGrey = new QPushButton("Greyscale", &window);
    btnGrey->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnGrey, &QPushButton::clicked, [&img]() {
        img.ConvertGreyscale();
    });
    currentHeight += BTN_ABOVE;

    // 3.5 Button to turn to negative:
    QPushButton *btnNeg = new QPushButton("Negative", &window);
    btnNeg->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnNeg, &QPushButton::clicked, [&img]() {
        img.ConvertNegative();
    });
    currentHeight += BTN_ABOVE;

    // 3.6 Button to zoom in:
    QPushButton *btnZoomIn = new QPushButton("Zoom in", &window);
    btnZoomIn->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnZoomIn, &QPushButton::clicked, [&img]() {
        img.ZoomIn();
    });
    currentHeight += BTN_ABOVE;

    // 3.7 Button to zoom out:
    QPushButton *btnZoomOut = new QPushButton("Zoom out", &window);
    btnZoomOut->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnZoomOut, &QPushButton::clicked, [&img]() {
        img.ZoomOut(2,2);
    });
    currentHeight += BTN_ABOVE;

    // 3.8 Button to rotate:
    QPushButton *btnRotate = new QPushButton("Rotate", &window);
    btnRotate->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnRotate, &QPushButton::clicked, [&img]() {
        img.Rotate();
    });
    currentHeight += BTN_ABOVE;
    currentHeight += SPACE;

    // 3.9 Separation line
    QFrame *line1 = new QFrame(&window);
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken); 
    line1->setGeometry(SPACE, currentHeight, BTN_WIDTH, 2);
    currentHeight += SPACE*2;


    // 4. FILTER BUTTONS SETUP

    // 4.1 Filter label 
    QLabel *title4 = new QLabel("Filters", &window);
    title4->setGeometry(SPACE, currentHeight, BTN_WIDTH, DESCRIPTION_HEIGHT);
    title4->setAlignment(Qt::AlignCenter);
    title4->setFont(font);
    currentHeight += TITLE_ABOVE;

    // 4.2 Button to apply gaussian filter:
    QPushButton *btnGaussian = new QPushButton("Gaussian", &window);
    btnGaussian->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnGaussian, &QPushButton::clicked, [&img]() {
        double kernel[3][3] = {{0.0625, 0.125, 0.0625}, {0.125, 0.25, 0.125} , {0.0625, 0.125, 0.0625}};
        img.ApplyFilter(kernel, false);
    });
    currentHeight += BTN_ABOVE;

    // 4.3 Button to apply laplacian filter:
    QPushButton *btnLaplacian = new QPushButton("Laplacian", &window);
    btnLaplacian->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnLaplacian, &QPushButton::clicked, [&img]() {
        double kernel[3][3] = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
        img.ApplyFilter(kernel, false);
    });
    currentHeight += BTN_ABOVE;

    // 4.4 Button to apply high-pass filter:
    QPushButton *btnHP = new QPushButton("High-Pass", &window);
    btnHP->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnHP, &QPushButton::clicked, [&img]() {
        double kernel[3][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};
        img.ApplyFilter(kernel, false);
    });
    currentHeight += BTN_ABOVE;

    // 4.5 Button to apply horizontal Prewitt filter:
    QPushButton *btnPrewittH = new QPushButton("Prewitt Horizontal", &window);
    btnPrewittH->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnPrewittH, &QPushButton::clicked, [&img]() {
        double kernel[3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
        img.ApplyFilter(kernel, true);
    });
    currentHeight += BTN_ABOVE;

    // 4.6 Button to apply vertical Prewitt filter:
    QPushButton *btnPrewittV = new QPushButton("Prewitt Vertical", &window);
    btnPrewittV->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnPrewittV, &QPushButton::clicked, [&img]() {
        double kernel[3][3] = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};
        img.ApplyFilter(kernel, true);
    });
    currentHeight += BTN_ABOVE;

    // 4.7 Button to apply horizontal Sobel filter:
    QPushButton *btnSobelH = new QPushButton("Sobel Horizontal", &window);
    btnSobelH->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnSobelH, &QPushButton::clicked, [&img]() {
        double kernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
        img.ApplyFilter(kernel, true);
    });
    currentHeight += BTN_ABOVE;

    // 4.8 Button to apply vertical Sobel filter:
    QPushButton *btnSobelV = new QPushButton("Sobel Vertical", &window);
    btnSobelV->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnSobelV, &QPushButton::clicked, [&img]() {
        double kernel[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
        img.ApplyFilter(kernel, true);
    });
    currentHeight += BTN_ABOVE;

    // 4.9 Button to set personalized filter:
    QPushButton *btnPersonalized = new QPushButton("Personalized", &window);
    btnPersonalized->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnPersonalized, &QPushButton::clicked, [&img]() {

        // Open new window for setting kernel matrix
        QWidget *matrixWindow = new QWidget;
        QTableWidget *matrix = new QTableWidget(3, 3);
        for (int i=0;i<3;++i) {
            for (int j=0;j<3;++j) {
                QTableWidgetItem *item = new QTableWidgetItem("0");
                matrix->setItem(i, j, item);
            }
        }

        // Button to apply new kernel
        QPushButton *btnApply = new QPushButton("Apply filter");
        QObject::connect(btnApply, &QPushButton::clicked, [&img, matrix]() {
            double kernel[3][3];
            for (int i=0;i<3;++i) {
                for (int j=0;j<3;++j) {
                    QTableWidgetItem *item = matrix->item(i, j);
                    if (item) {
                        QString text = item->text();
                        double value = text.toDouble(); 
                        kernel[i][j] = value;
                    }
                }
            }
            img.ApplyFilter(kernel, false);
        });

        // Launch new window
        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(matrix);
        layout->addWidget(btnApply);
        matrixWindow->setLayout(layout);
        matrixWindow->setWindowTitle("New filter");
        matrixWindow->resize(340, 180);
        matrixWindow->show();

    });
    currentHeight += BTN_ABOVE;
    currentHeight += SPACE;

    // 4.10 Separation line
    QFrame *line2 = new QFrame(&window);
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken); 
    line2->setGeometry(SPACE, currentHeight, BTN_WIDTH, 2);
    currentHeight += SPACE*2;


    // 5. HISTOGRAM OPERATIONS BUTTONS SETUP

    // 5.1 Histogram label
    QLabel *title5 = new QLabel("Histograms", &window);
    title5->setGeometry(SPACE, currentHeight, BTN_WIDTH, DESCRIPTION_HEIGHT);
    title5->setAlignment(Qt::AlignCenter);
    title5->setFont(font);
    currentHeight += TITLE_ABOVE;
    
    // 5.2 Button to show histogram:
    QPushButton *btnHist = new QPushButton("Show grey histogram", &window);
    btnHist->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnHist, &QPushButton::clicked, [&img]() {
        img.ShowHistogram();
    });
    currentHeight += BTN_ABOVE;

    // 5.3 Button to equalize histogram:
    QPushButton *btnEqual = new QPushButton("Equalize histogram", &window);
    btnEqual->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnEqual, &QPushButton::clicked, [&img]() {
        img.EqualizeImgHistogram();
    });
    currentHeight += BTN_ABOVE;

    // 5.4 Button to better equalize colored images:
    QPushButton *btnLab = new QPushButton("L*a*b* equalization", &window);
    btnLab->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnLab, &QPushButton::clicked, [&img]() {
        if (img.GetGreyFlag()) {
            img.EqualizeImgHistogram();
        } else {
            img.EqualizeTroughLAB();
        }
    });
    currentHeight += BTN_ABOVE;
    currentHeight += SPACE;

    // 5.5 Separation line
    QFrame *line3 = new QFrame(&window);
    line3->setFrameShape(QFrame::HLine);
    line3->setFrameShadow(QFrame::Sunken); 
    line3->setGeometry(SPACE, currentHeight, BTN_WIDTH, 2);
    currentHeight += SPACE*2;


    // 6. PARAMETERS SETUP

    // 6.1 Parameters label
    QLabel *title6 = new QLabel("Parameters", &window);
    title6->setGeometry(SPACE, currentHeight, BTN_WIDTH, DESCRIPTION_HEIGHT);
    title6->setAlignment(Qt::AlignCenter);
    title6->setFont(font);
    currentHeight += TITLE_ABOVE;

    // 6.2 Quantization slider
    // Description for quantization slider:
    QLabel *title7 = new QLabel("Colors for quantization:", &window);
    title7->setGeometry(SPACE, currentHeight, BTN_WIDTH, SLIDER_TITLE_HEIGHT);
    title7->setAlignment(Qt::AlignCenter);
    currentHeight += SLIDER_TITLE_HEIGHT;
    // Slider:
    QSlider *sliderQtz = new QSlider(Qt::Horizontal, &window);
    sliderQtz->setRange(1, 256);
    sliderQtz->setValue(265);
    sliderQtz->setGeometry(SPACE, currentHeight, SLIDER_WIDTH, SLIDER_HEIGHT);
    QObject::connect(sliderQtz, &QSlider::sliderReleased, [&img, sliderQtz]() {
        img.AdjustQuantization(sliderQtz->value());
    });
    // Label to show slider value:
    QLabel *num1 = new QLabel(QString::number(sliderQtz->value()), &window);
    num1->setGeometry(SLIDER_WIDTH+SPACE, currentHeight, SLIDER_NUM_WIDTH, SLIDER_HEIGHT);
    num1->setAlignment(Qt::AlignCenter);
    QObject::connect(sliderQtz, &QSlider::valueChanged, [num1](int value) {
        num1->setText(QString::number(value));
    });
    currentHeight += SLIDER_HEIGHT;
    
    // 6.3 Brightness slider
    // Description for brightness slider:
    QLabel *title8 = new QLabel("Brightness:", &window);
    title8->setGeometry(SPACE, currentHeight, BTN_WIDTH, SLIDER_TITLE_HEIGHT);
    title8->setAlignment(Qt::AlignCenter);
    currentHeight += SLIDER_TITLE_HEIGHT;
    // Slider:
    QSlider *sliderBright = new QSlider(Qt::Horizontal, &window);
    sliderBright->setRange(-255, 255);
    sliderBright->setValue(0);
    sliderBright->setGeometry(SPACE, currentHeight, SLIDER_WIDTH, SLIDER_HEIGHT);
    QObject::connect(sliderBright, &QSlider::sliderReleased, [&img, sliderBright]() {
        img.AdjustBrightness(sliderBright->value());
    });
    // Label to show slider value:
    QLabel *num2 = new QLabel(QString::number(sliderBright->value()), &window);
    num2->setGeometry(SLIDER_WIDTH+SPACE, currentHeight, SLIDER_NUM_WIDTH, SLIDER_HEIGHT);
    num2->setAlignment(Qt::AlignCenter);
    QObject::connect(sliderBright, &QSlider::valueChanged, [num2](int value) {
        num2->setText(QString::number(value));
    });
    currentHeight += SLIDER_HEIGHT;

    // 6.4 Contrast slider
    // Description for contrast slider:
    QLabel *title9 = new QLabel("Contrast:", &window);
    title9->setGeometry(SPACE, currentHeight, BTN_WIDTH, SLIDER_TITLE_HEIGHT);
    title9->setAlignment(Qt::AlignCenter);
    currentHeight += SLIDER_TITLE_HEIGHT;
    // Slider:
    QSlider *sliderCont = new QSlider(Qt::Horizontal, &window);
    sliderCont->setRange(0, 255);
    sliderCont->setValue(100);
    sliderCont->setGeometry(SPACE, currentHeight, SLIDER_WIDTH, SLIDER_HEIGHT);
    // Dividing slider into frequency sections
    auto mapSliderValue = [](int sliderValue) -> float {
        if (sliderValue <= 200) {
            // Maps 0-100 to 0-2
            return sliderValue / 100.0f;
        } else {
            // Maps 100-255 to 2-255
            return 2.0f + ((sliderValue - 200) / 55.0f) * 254.0f;
        }
    };
    QObject::connect(sliderCont, &QSlider::sliderReleased, [&img, sliderCont, mapSliderValue]() {
        float contrastValue = mapSliderValue(sliderCont->value());
        img.AdjustContrast(contrastValue);
    });
    // Label to show slider value:
    QLabel *num3 = new QLabel(QString::number(mapSliderValue(sliderCont->value()), 'f', 2), &window);
    num3->setGeometry(SLIDER_WIDTH+SPACE, currentHeight, SLIDER_NUM_WIDTH, SLIDER_HEIGHT);
    num3->setAlignment(Qt::AlignCenter);
    QObject::connect(sliderCont, &QSlider::valueChanged, [num3, mapSliderValue](float value) {
        if (value <= 200) {
            num3->setText(QString::number(mapSliderValue(value), 'f', 2));
        } else {
            num3->setText(QString::number(mapSliderValue(value), 'f', 0));
        }
    });
    currentHeight += SLIDER_HEIGHT;
    currentHeight += SPACE;

    // 6.5 Separation line
    QFrame *line4 = new QFrame(&window);
    line4->setFrameShape(QFrame::HLine);
    line4->setFrameShadow(QFrame::Sunken); 
    line4->setGeometry(SPACE, currentHeight, BTN_WIDTH, 2);
    currentHeight += SPACE*3;


    // 7. OTHER BUTTONS

    // 7.1 Button for reseting current image:
    QPushButton *btnReset = new QPushButton("Reset", &window);
    btnReset->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnReset, &QPushButton::clicked, [&img]() {
        img.Reset();
    });
    currentHeight += BTN_ABOVE;

    // 7.2 Button for saving current image as JPEG file:
    QPushButton *btnSave = new QPushButton("Save as JPEG", &window);
    btnSave->setGeometry(SPACE, currentHeight, BTN_WIDTH, BTN_HEIGHT);
    QObject::connect(btnSave, &QPushButton::clicked, [&img]() {
        img.Save();
    });
    currentHeight += BTN_ABOVE;


    // 8. ADJUST AND LAUCH APPLICATION

    if (mainImg.rows < currentHeight+SPACE) {
        windowHeight = currentHeight+SPACE;
    }
    window.setFixedSize(windowWidth, windowHeight);
    img.SetMinHeight(currentHeight+SPACE);

    window.show();
    return app.exec();
}
