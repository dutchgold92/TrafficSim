#include "graphicscellitem.h"

GraphicsCellItem::GraphicsCellItem(Cell *cell, qreal x, qreal y, qreal width, qreal height) :
    QGraphicsRectItem(x, y, width, height)
{
    this->cell = cell;

    if(this->has_vehicle())
    {
        this->setBrush(this->vehicle_brush());
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    }
    else
    {
        this->setBrush(this->empty_cell_brush());
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
    if(cell->has_vehicle())
    {
        if(selected)
            this->setBrush(this->focused_vehicle_brush());
        else
            this->setBrush(this->vehicle_brush());
    }
}

QBrush GraphicsCellItem::vehicle_brush()
{
    return QBrush(Qt::green);
}

QBrush GraphicsCellItem::focused_vehicle_brush()
{
    return QBrush(Qt::red);
}

QBrush GraphicsCellItem::empty_cell_brush()
{
    return QBrush(Qt::white);
}

GraphicsCellItem* qgraphicsitem_cast(QGraphicsItem *item)
{
    return (GraphicsCellItem*)item;
}
