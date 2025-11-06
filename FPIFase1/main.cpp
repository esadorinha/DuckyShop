#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include "teste.hpp"

#define DESCRIPTION_AREA_V 15
#define COMMAND_AREA_V 180
#define COMMAND_AREA_H 180
#define SPACE 5
#define BTN_SIZE_V 20
#define BTN_SIZE_H 160


// AUXILIAR FUNCTION

QImage MatToQImage(const cv::Mat &mat) {
    // Copy input Mat
    const uchar *qImageBuffer = (const uchar*)mat.data;
    // Create QImage with same dimensions as input Mat
    QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return img.rgbSwapped();
}


// EDITING/SAVING FUNCTIONS

void Reset(QLabel *label, cv::Mat &currentImg, cv::Mat &quantBuffer, cv::Mat &mainImg) {
    currentImg = mainImg.clone();
    quantBuffer = currentImg;
    QImage qImg = MatToQImage(mainImg);
    label->setPixmap(QPixmap::fromImage(qImg));
}

void MirrorHorizontally(QLabel *label, cv::Mat &currentImg, cv::Mat &quantBuffer) {
    cv::Mat newImage = InvertHorizontally(quantBuffer);
    quantBuffer = newImage.clone();
    newImage = InvertHorizontally(currentImg);
    currentImg = newImage.clone();
    QImage qImg = MatToQImage(newImage);
    label->setPixmap(QPixmap::fromImage(qImg));
}

void MirrorVertically(QLabel *label, cv::Mat &currentImg, cv::Mat &quantBuffer) {
    cv::Mat newImage = InvertVertically(quantBuffer);
    quantBuffer = newImage.clone();
    newImage = InvertVertically(currentImg);
    currentImg = newImage.clone();
    QImage qImg = MatToQImage(newImage);
    label->setPixmap(QPixmap::fromImage(qImg));
}

void TurnToGrey(QLabel *label, cv::Mat &currentImg) {
    cv::Mat newImage = GreyScale(currentImg);
    currentImg = newImage.clone();
    QImage qImg = MatToQImage(newImage);
    label->setPixmap(QPixmap::fromImage(qImg));
}

void Quantize(QLabel *label, cv::Mat &currentImg, cv::Mat &quantBuffer, int numShades) {
    cv::Mat newImage = Quantization(GreyScale(quantBuffer), numShades);
    currentImg = newImage.clone();
    QImage qImg = MatToQImage(newImage);
    label->setPixmap(QPixmap::fromImage(qImg));
}

void Save(cv::Mat &currentImg) {
    cv::imwrite("DuckyShop.jpeg", currentImg);
}


// MAIN FUNCTION

int main(int argc, char *argv[]) {
    // Check provided parameters
    if (argc < 2) {
        std::cerr << "ERROR: you must provide image path!" << std::endl;
        std::cerr << "Usage example: " << argv[0] << " <caminho_da_imagem>" << std::endl;
        return -1;
    }

    // Open main image
    cv::Mat mainImg = cv::imread(argv[1]);
    if (mainImg.empty()) {
        std::cout << "Failed to open image!" << std::endl;
        return -1;
    }
    // Set current image (editing variable) and buffer for quantization
    cv::Mat currentImg = mainImg;
    cv::Mat quantBuffer = currentImg;

    // Set button's position
    int commandAreaPos = mainImg.cols*2 + SPACE*3;

    // Set window sizes based on chosen image
    int windowSizeH = commandAreaPos + COMMAND_AREA_H;
    int windowSizeV = DESCRIPTION_AREA_V + mainImg.rows + SPACE*3;
    if (mainImg.rows < COMMAND_AREA_V) {
        windowSizeV += COMMAND_AREA_V - mainImg.rows;
    }

    // Inicialize Qt application
    QApplication app(argc, argv);

    // Inicialize application window
    QWidget window;
    window.setWindowTitle("Ducky Shop");
    window.setFixedSize(windowSizeH, windowSizeV);

    // Prepare original images to lauch on window
    QImage qMainImg = MatToQImage(mainImg);
    QImage qCurImg = MatToQImage(currentImg);

    QLabel *label1 = new QLabel(&window);
    QLabel *label2 = new QLabel(&window);
    label1->setPixmap(QPixmap::fromImage(qMainImg));
    label2->setPixmap(QPixmap::fromImage(qCurImg));

    // Set position of images to the left
    label1->setGeometry(SPACE, SPACE*2+DESCRIPTION_AREA_V, qMainImg.width(), qMainImg.height());
    label2->setGeometry(SPACE*2+qMainImg.width(), SPACE*2+DESCRIPTION_AREA_V, qCurImg.width(), qCurImg.height());

    // Prepare images descriptions
    QLabel *labelTitle1 = new QLabel("Original", &window);
    labelTitle1->setGeometry(SPACE, SPACE, qMainImg.width(), DESCRIPTION_AREA_V);
    labelTitle1->setAlignment(Qt::AlignCenter);
    QLabel *labelTitle2 = new QLabel("Current", &window);
    labelTitle2->setGeometry(SPACE*2+qMainImg.width(), SPACE, qMainImg.width(), DESCRIPTION_AREA_V);
    labelTitle2->setAlignment(Qt::AlignCenter);


    // Edition label
    QLabel *labelTitle3 = new QLabel("Edition options", &window);
    labelTitle3->setGeometry(commandAreaPos+SPACE, SPACE, BTN_SIZE_H, DESCRIPTION_AREA_V);
    labelTitle3->setAlignment(Qt::AlignCenter);

    // Create buttons/sliders for calling functions
    // Reset current image:
    QPushButton *btnReset = new QPushButton("Reset", &window);
    btnReset->setGeometry(commandAreaPos+SPACE, DESCRIPTION_AREA_V+SPACE*2, BTN_SIZE_H, BTN_SIZE_V);
    QObject::connect(btnReset, &QPushButton::clicked, [&currentImg, &quantBuffer, &mainImg, label2]() {
        Reset(label2, currentImg, quantBuffer, mainImg);
    });

    // Mirror horizontally:
    QPushButton *btnMirrorH = new QPushButton("Mirror horizontally", &window);
    btnMirrorH->setGeometry(commandAreaPos+SPACE, DESCRIPTION_AREA_V+SPACE*3+BTN_SIZE_V*1, BTN_SIZE_H, BTN_SIZE_V);
    QObject::connect(btnMirrorH, &QPushButton::clicked, [&currentImg, &quantBuffer, label2]() {
        MirrorHorizontally(label2, currentImg, quantBuffer);
    });

    // Mirror vertically:
    QPushButton *btnMirrorV = new QPushButton("Mirror vertically", &window);
    btnMirrorV->setGeometry(commandAreaPos+SPACE, DESCRIPTION_AREA_V+SPACE*4+BTN_SIZE_V*2, BTN_SIZE_H, BTN_SIZE_V);
    QObject::connect(btnMirrorV, &QPushButton::clicked, [&currentImg, &quantBuffer, label2]() {
        MirrorVertically(label2, currentImg, quantBuffer);
    });

    // Turn to greyscale:
    QPushButton *btnGrey = new QPushButton("Greyscale", &window);
    btnGrey->setGeometry(commandAreaPos+SPACE, DESCRIPTION_AREA_V+SPACE*5+BTN_SIZE_V*3, BTN_SIZE_H, BTN_SIZE_V);
    QObject::connect(btnGrey, &QPushButton::clicked, [&currentImg, label2]() {
        TurnToGrey(label2, currentImg);
    });
    
    // Label to describe quantization slider:
    QLabel *labelTitle4 = new QLabel("Colors for quantization:", &window);
    labelTitle4->setGeometry(commandAreaPos+SPACE, DESCRIPTION_AREA_V+SPACE*6+BTN_SIZE_V*4, BTN_SIZE_H, BTN_SIZE_V);
    labelTitle4->setAlignment(Qt::AlignCenter);
    // Quantize (slider):
    QSlider *slider = new QSlider(Qt::Horizontal, &window);
    slider->setRange(1, 256);
    slider->setValue(265);
    slider->setGeometry(commandAreaPos+SPACE, DESCRIPTION_AREA_V+SPACE*6+2+BTN_SIZE_V*5, BTN_SIZE_H-30, BTN_SIZE_V);
    QObject::connect(slider, &QSlider::sliderReleased, [&currentImg, &quantBuffer, label2, slider]() {
        Quantize(label2, currentImg, quantBuffer, slider->value());
    });
    // Label to show slider value
    QLabel *label = new QLabel(QString::number(slider->value()), &window);
    label->setGeometry(commandAreaPos+SPACE+BTN_SIZE_H-30, DESCRIPTION_AREA_V+SPACE*6+2+BTN_SIZE_V*5, 25, BTN_SIZE_V);
    QObject::connect(slider, &QSlider::valueChanged, [label](int value) {
        label->setText(QString::number(value));
    });
    
    // Save current image as JPEG file:
    QPushButton *btnSave = new QPushButton("Save as JPEG", &window);
    btnSave->setGeometry(commandAreaPos+SPACE, DESCRIPTION_AREA_V+SPACE*7+2+BTN_SIZE_V*6, BTN_SIZE_H, BTN_SIZE_V);
    QObject::connect(btnSave, &QPushButton::clicked, [&currentImg]() {
        Save(currentImg);
    });


    // Show window and run
    window.show();
    return app.exec();
}
