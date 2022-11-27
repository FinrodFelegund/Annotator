//
// Created by Daniel Pietsch on 27.11.22.
//

#include "MiniMap.h"
#include <QPainter>

MiniMap::MiniMap(QPixmap map, QWidget *parent) : QWidget(parent)
{
    this->_map = map;
    _ratio = (float)map.width() / (float)map.height();
    QSizePolicy policy;
    policy.setHeightForWidth(true);
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    this->setSizePolicy(policy);
    this->setFixedWidth(250);
    this->setFixedHeight(250);
}

void MiniMap::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(!_map.isNull())
    {
        painter.drawPixmap(1, 1, width(), height(), _map);
    }
}

QSize MiniMap::sizeHint() const
{
    QSize size(0, 0);
    if(_map.isNull())
    {
        return size;
    }
    int baseSize = 250;
    if(_map.width() > _map.height())
        {
            size = QSize(baseSize, baseSize / _ratio);
        }
    else
    {
        size = QSize(baseSize * _ratio, baseSize);
    }

    qDebug() << size;
    return QSize(250, 250);

}

int MiniMap::heightForWidth(int w) const
{

    return w / _ratio;
}
