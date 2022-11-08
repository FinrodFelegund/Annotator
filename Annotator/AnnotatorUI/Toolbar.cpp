//
// Created by Daniel Pietsch on 06.11.22.
//

#include "Toolbar.h"

Toolbar::Toolbar(QWidget *parent)
{
    //setParent(parent);
    setOrientation(Qt::Vertical);
    this->setMinimumWidth(40);

    QIcon *icon = new QIcon();
    icon->addPixmap(QPixmap(":/Icons/clicker.png"));
    QAction *action = new QAction( *icon, tr("Panning"), this);
    action->setObjectName("Panning");
    action->setCheckable(true);
    this->addAction(action);

    icon->addPixmap(QPixmap(":/Icons/pencil-cursor.png"));
    action = new QAction(*icon, tr("Drawing"), this);
    action->setObjectName("Drawing");
    action->setCheckable(true);
    this->addAction(action);

    icon->addPixmap(QPixmap(":Icons/exit.png"));
    action = new QAction(*icon, tr("Close"), this);
    action->setObjectName("Exit");
    this->addAction(action);

}

