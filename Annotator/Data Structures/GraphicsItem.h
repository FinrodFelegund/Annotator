//
// Created by Daniel Pietsch on 01.09.22.
//

#ifndef ANNOTATOR_GRAPHICSITEM_H
#define ANNOTATOR_GRAPHICSITEM_H
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class GraphicsItem : public QGraphicsItem
{
public:
    GraphicsItem(QPixmap map, int scaleFactor, int level, int cx, int cy);
    ~GraphicsItem();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void opacityChanged();
    void print();
    int getX();
    int getY();
    int getLevel();
    void updatePixmap(QPixmap map);
    void setOpacity(qreal opacity);

private:
    QPixmap _map;
    QRectF _boundingRect;
    int _scaleFactor;
    int _level;
    int _cx;
    int _cy;
    qreal _opacity;
};


#endif //ANNOTATOR_GRAPHICSITEM_H
