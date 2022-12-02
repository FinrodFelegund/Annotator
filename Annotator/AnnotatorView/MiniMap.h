//
// Created by Daniel Pietsch on 27.11.22.
//

#ifndef ANNOTATOR_MINIMAP_H
#define ANNOTATOR_MINIMAP_H
#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>

class MiniMap : public QWidget
{
    Q_OBJECT
public:
    MiniMap(QWidget *parent);
    QSize sizeHint() const;
    int heightForWidth(int w) const;
    void setPixmap(QPixmap map);
    void setDownSample(int downSample);
    void reset();

public slots:
    void updateFieldOfView(QRectF rect);

signals:
    void mapClicked(QPointF pos);


protected:
    void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
private:
    QRectF _fieldOfView;
    QPixmap _map;
    float _ratio;
    int _downSample;
    int _baseSize;
};


#endif //ANNOTATOR_MINIMAP_H
