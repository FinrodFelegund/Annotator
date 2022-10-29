//
// Created by Daniel Pietsch on 08.08.22.
//

#ifndef ANNOTATOR_ANNOTATORMAINWINDOW_H
#define ANNOTATOR_ANNOTATORMAINWINDOW_H
#include <QMainWindow>
#include <QHBoxLayout>
#include "AnnotatorView/AnnotatorViewer.h"
#include "Reader/WholeSlideImageReader.h"

class AnnotatorMainWindow : public QMainWindow
{
public:
    AnnotatorMainWindow(QObject *parent = nullptr);
    ~AnnotatorMainWindow() noexcept;
    void setUpUi();
    void setUpGraphics();

public slots:
    void openImage();

private:
    std::shared_ptr<AnnotatorViewer> _view;
    std::shared_ptr<WholeSlideImageReader> _img;
    QWidget *_centralWidget;
    QHBoxLayout *_horizontalLayout;

    QAction *_openImage;


};


#endif //ANNOTATOR_ANNOTATORMAINWINDOW_H
