#include <QApplication>
#include "AnnotatorMainWindow.h"
#include <QScreen>



int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    AnnotatorMainWindow window;
    window.move(window.screen()->geometry().center() - window.frameGeometry().center());
    window.show();

    return QApplication::exec();
}
