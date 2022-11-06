//
// Created by Daniel Pietsch on 08.08.22.
//

#ifndef ANNOTATOR_ANNOTATORMAINWINDOW_H
#define ANNOTATOR_ANNOTATORMAINWINDOW_H
#include <QMainWindow>
#include <QHBoxLayout>
#include "../AnnotatorView/AnnotatorViewer.h"
#include "Toolbar.h"

class AnnotatorMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    AnnotatorMainWindow(QObject *parent = nullptr);
    ~AnnotatorMainWindow() noexcept;
    void setUpUi();
    void setUpGraphics();
    std::shared_ptr<AnnotatorViewer> getView();

public slots:
    void openImage();

signals:
    void initializeImage(std::string fileName);

private:
    std::shared_ptr<AnnotatorViewer> _view;
    QWidget *_centralWidget;
    QHBoxLayout *_horizontalLayout;
    Toolbar *bar;
    QAction *_openImage;
    QString _currentFileName;


};


#endif //ANNOTATOR_ANNOTATORMAINWINDOW_H
