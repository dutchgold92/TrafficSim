#ifndef GRAPHICSCELLITEM_H
#define GRAPHICSCELLITEM_H

#include <QGraphicsRectItem>
#include <vehicle.h>
#include <cell.h>
#include <QBrush>
#include <QPen>

class GraphicsCellItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
private:
    Cell *cell;
public:
    explicit GraphicsCellItem(Cell *cell, qreal x, qreal y, qreal width, qreal height);
    bool is_junction();
    bool has_vehicle();
signals:
    
public slots:
    
};

#endif // GRAPHICSCELLITEM_H
