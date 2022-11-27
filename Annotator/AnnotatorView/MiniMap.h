//
// Created by Daniel Pietsch on 27.11.22.
//

#ifndef ANNOTATOR_MINIMAP_H
#define ANNOTATOR_MINIMAP_H
#include <QWidget>
#include <QPixmap>

class MiniMap : public QWidget
{
    Q_OBJECT
public:
    MiniMap(QPixmap map, QWidget *parent);
    QSize sizeHint() const;
    int heightForWidth(int w) const;

protected:
    void paintEvent(QPaintEvent *event);
private:
    QRectF _fieldOfView;
    QPixmap _map;
    float _ratio;
};


#endif //ANNOTATOR_MINIMAP_H
