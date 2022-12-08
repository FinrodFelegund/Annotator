//
// Created by Daniel Pietsch on 27.11.22.
//

#include "MiniMap.h"
#include <QPainter>

MiniMap::MiniMap(QWidget *parent) : QWidget(parent)
{
    setEnabled(false);
    _map = QPixmap();
    _ratio = 0;
    _downSample = 0;
    _baseSize = 250;
    _clicked = false;
    QSizePolicy policy;
    policy.setHeightForWidth(true);
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    this->setSizePolicy(policy);
}

void MiniMap::setPixmap(QPixmap map)
{
    _map = map;
    _ratio = (float)_map.width() / _map.height();
}

void MiniMap::setDownSample(int downSample)
{
    _downSample = downSample;
}

void MiniMap::reset()
{
    _map = QPixmap();
    _ratio = 0;
    _downSample = 0;
    _fieldOfView = QRect(0, 0, 0, 0);
    setEnabled(false);
}

void MiniMap::initalize()
{
    setEnabled(true);
    if(!_map.isNull())
    {
        _fieldOfView = QRectF(1, 1, width() - 2, height() - 2);
        update();
    }
}

void MiniMap::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    if(!_map.isNull())
    {
        //qDebug() << "Drawing: " << width() << " " << height();
        painter.drawPixmap(0, 0, this->width(), this->height(), _map);
    }

    if(!_fieldOfView.isNull())
    {
        painter.setPen(QPen(Qt::black, 3));
        painter.drawRect(_fieldOfView);
    }
}

void MiniMap::updateFieldOfView(QRectF rect)
{


    if(!rect.isNull())
    {
        //bring the fieldofView in Minimap coordinates by scaling it down
        _fieldOfView = QRectF(rect.x() / _downSample, rect.y() / _downSample, rect.width() / _downSample, rect.height() / _downSample);
        _fieldOfView = QRectF(_fieldOfView.x() / _map.width() * width(), _fieldOfView.y() / _map.height() * height(), _fieldOfView.width() / _map.width() * width(), _fieldOfView.height() / _map.height() * height());


        if(_fieldOfView.x() < 0 && _fieldOfView.y() < 0 && _fieldOfView.width() > width() && _fieldOfView.height() > height())
        {
            _fieldOfView = QRectF(1, 1, width() - 2, height() - 2);
        } else if(_fieldOfView.height() > height())
        {
            _fieldOfView = QRectF(_fieldOfView.x() + 1, 1, _fieldOfView.width() - 2, height() - 2);
        } else if(_fieldOfView.width() > width())
        {
            _fieldOfView = QRectF(1, _fieldOfView.y() + 1, width() - 2, _fieldOfView.height() - 2);
        } else
        {
            _fieldOfView = QRectF(_fieldOfView.x() + 1, _fieldOfView.y() + 1, _fieldOfView.width() - 2, _fieldOfView.height() - 2);
        }

        update();
    }
}

QSize MiniMap::sizeHint() const
{

    int width = 0, height = 0;
    if(_map.width() >= _map.height())
    {
        width = 250;
        height = 250 / _ratio;
    } else
    {
        height = 250;
        width = 250 * _ratio;
    }

    QSize size(width, height);
    return size;

}

int MiniMap::heightForWidth(int w) const
{
    //qDebug() << "Width: " << w;
    return w / _ratio;
}

void MiniMap::mousePressEvent(QMouseEvent *event)
{
    QPointF point = event->pos();
    point = QPointF(point.x() / width() * _map.width(), point.y() / height() * _map.height());
    point = QPointF(point.x() * _downSample, point.y() * _downSample);

    emit mapClicked(point);

    _clicked = true;
    _currentPos = event->pos();

}

void MiniMap::mouseMoveEvent(QMouseEvent *event)
{
    if(_clicked)
    {

        QPointF tmp = QPointF((double)_currentPos.x() / width(), (double)_currentPos.y() / height());
        tmp = QPointF(tmp.x() * _map.width(), tmp.y() * _map.height());
        tmp = QPointF(tmp.x() * _downSample, tmp.y() * _downSample);


        /*//scale both points up
        QPointF delta = event->pos();
        delta = QPointF(delta.x() / width(), delta.y() / height());
        delta = QPointF(delta.x() * _map.width(), delta.y() / _map.height());
        delta = QPointF(delta.x() * _downSample, delta.y() / _downSample);*/

        emit mapDragged(tmp);
        _currentPos = event->pos();
    }
}

void MiniMap::mouseReleaseEvent(QMouseEvent *event)
{
    _clicked = false;
}
