//
// Created by Daniel Pietsch on 01.09.22.
//

#include "GraphicsItem.h"

//this should do for now

GraphicsItem::GraphicsItem(QPixmap map, int scaleFactor, int level, int cx, int cy)
{
    if(!map.isNull())
    {
        _map = map;
    }


    _scaleFactor = scaleFactor;

    _boundingRect = QRectF(0, 0, _map.width() * _scaleFactor, _map.height() * _scaleFactor);

    _level = level;

    _cx = cx;
    _cy = cy;
}

GraphicsItem::~GraphicsItem()
{
    _cx = _cy = _level = _scaleFactor = 0;
}

QRectF GraphicsItem::boundingRect() const
{
    return _boundingRect;
}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setOpacity(_opacity);
    painter->drawPixmap(option->exposedRect, _map, option->exposedRect);
}

void GraphicsItem::opacityChanged()
{
    update();
}

void GraphicsItem::print()
{
    qDebug() << _boundingRect;
}

int GraphicsItem::getX()
{
    return _cx;
}

int GraphicsItem::getY()
{
    return _cy;
}

int GraphicsItem::getLevel()
{
    return _level;
}

void GraphicsItem::updatePixmap(QPixmap map)
{
    this->_map = map;
    this->update();
}

void GraphicsItem::setOpacity(qreal opacity)
{
    _opacity = opacity;
}
