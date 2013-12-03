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
        if(this->is_junction())
            this->setBrush(QBrush(Qt::red));
        else
            this->setBrush(QBrush(Qt::black));

        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
    }
}

bool GraphicsCellItem::is_junction()
{
    return(this->cell->is_junction());
}

bool GraphicsCellItem::has_vehicle()
{
    return(this->cell->has_vehicle());
}

Vehicle *GraphicsCellItem::get_vehicle()
{
    return(this->cell->get_vehicle());
}

void GraphicsCellItem::setSelected(bool selected)
{
    if(selected)
        this->setBrush(QBrush(Qt::green));
    else
        this->setBrush(QBrush(Qt::blue));
}

GraphicsCellItem* qgraphicsitem_cast(QGraphicsItem *item)
{
    return (GraphicsCellItem*)item;
}
