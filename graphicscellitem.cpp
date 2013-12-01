#include "graphicscellitem.h"

GraphicsCellItem::GraphicsCellItem(Cell *cell, qreal x, qreal y, qreal width, qreal height) :
    QGraphicsRectItem(x, y, width, height)
{
    this->cell = cell;

    if(this->has_vehicle())
    {
        this->setBrush(QBrush(Qt::blue));
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    }
    else
    {
        this->setBrush(QBrush(Qt::black));
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
    }

    if(this->is_junction())
            this->setPen(QPen(Qt::red));
}

bool GraphicsCellItem::is_junction()
{
    return(this->cell->is_junction());
}

bool GraphicsCellItem::has_vehicle()
{
    return(this->cell->has_vehicle());
}
