//
// Created by Daniel Pietsch on 01.09.22.
//

#ifndef ANNOTATOR_GRAPHICSITEM_H
#define ANNOTATOR_GRAPHICSITEM_H
#include <QGraphicsItem>

class GraphicsItem : public QGraphicsItem
{
public:
    GraphicsItem();
    ~GraphicsItem();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};


#endif //ANNOTATOR_GRAPHICSITEM_H
