//
// Created by Daniel Pietsch on 07.11.22.
//

#ifndef ANNOTATOR_DOCKWIDGET_H
#define ANNOTATOR_DOCKWIDGET_H
#include <QDockWidget>

class DockWidget : public QDockWidget{

public:
    DockWidget(QWidget *parent = nullptr);

protected:
    virtual void resizeEvent(QResizeEvent *event);


private:
    int _maxWidth;
};


#endif //ANNOTATOR_DOCKWIDGET_H
