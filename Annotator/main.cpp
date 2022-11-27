#include <QApplication>
#include "AnnotatorApplication/AnnotatorController.h"
#include "AnnotatorUI/AnnotatorMainWindow.h"
#include "Reader/WholeSlideImageReader.h"
#include <QImage>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    std::shared_ptr<AnnotatorMainWindow> window = std::make_shared<AnnotatorMainWindow>();
    std::shared_ptr<AnnotatorController> controller = std::make_shared<AnnotatorController>();
    controller->setWindow(window);
    controller->showUI();
    /*WholeSlideImageReader reader;
    reader.initializeImage("/Users/danielpietsch/Documents/Bilder/WholeSlideImages/Leica-1.scn");
    if(!reader.isValid())
    {
        qDebug() << "Not valid";
        return 0;
    }

    unsigned char *buf = reader.readDataFromImage(0, 0, 208, 601, 4);
    QImage img1(buf, 208, 601, 3 * 208, QImage::Format_RGB888);
    QLabel *label1 = new QLabel;
    label1->setPixmap(QPixmap::fromImage(img1));
    label1->show();
    img1.save("test1.jpg", nullptr, 80);

    unsigned char *buf2 = reader.readDataFromImage(15000, 50000, 1000, 1000, 0);
    QImage img2(buf2, 1000, 1000, 3 * 1000, QImage::Format_RGB888);
    QLabel *label2 = new QLabel;
    label2->setPixmap(QPixmap::fromImage(img2));
    label2->show();
    img2.save("test2.jpg", nullptr, 80);*/


    return QApplication::exec();

}
