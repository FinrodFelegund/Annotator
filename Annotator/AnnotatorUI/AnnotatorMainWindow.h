//
// Created by Daniel Pietsch on 08.08.22.
//

#ifndef ANNOTATOR_ANNOTATORMAINWINDOW_H
#define ANNOTATOR_ANNOTATORMAINWINDOW_H
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "../AnnotatorView/AnnotatorViewer.h"
#include "Toolbar.h"
#include "Dockwidget.h"

class AnnotatorMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    AnnotatorMainWindow(QObject *parent = nullptr);
    ~AnnotatorMainWindow() noexcept;
    void setUpUi();
    void setUpGraphics();
    std::shared_ptr<AnnotatorViewer> getView();
    Toolbar* getToolBar();

public slots:
    void openImage();

signals:
    void initializeImage(std::string fileName);

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    std::shared_ptr<AnnotatorViewer> _view;
    QWidget *_centralWidget;
    QHBoxLayout *_horizontalLayout;
    Toolbar *_bar;


    DockWidget *_leftDockwidget;
    QVBoxLayout *_leftLayout;
    DockWidget *_rightDockwidget;
    QAction *_openImage;
    QString _currentFileName;


};


#endif //ANNOTATOR_ANNOTATORMAINWINDOW_H
