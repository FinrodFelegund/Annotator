#include <QApplication>
#include "AnnotatorApplication/AnnotatorController.h"
#include "AnnotatorUI/AnnotatorMainWindow.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    std::shared_ptr<AnnotatorMainWindow> window = std::make_shared<AnnotatorMainWindow>();
    std::shared_ptr<AnnotatorController> controller = std::make_shared<AnnotatorController>();
    controller->setWindow(window);
    controller->showUI();

    return a.exec();

}
