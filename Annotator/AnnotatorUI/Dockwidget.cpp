//
// Created by Daniel Pietsch on 07.11.22.
//

#include "Dockwidget.h"

DockWidget::DockWidget(QWidget *parent) : QDockWidget(parent)
{
    _maxWidth = 200;

    //setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);
    this->setTitleBarWidget(new QWidget(this));


}

void DockWidget::resizeEvent(QResizeEvent *event)
{
    //qDebug() << "Hello there";

}

