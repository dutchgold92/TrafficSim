#include "graphicscellitem.h"

/**
 * @brief GraphicsCellItem::GraphicsCellItem Initialises a GraphicsCellItem object.
 * @param cell Cell object to represent.
 * @param x Coordinate on the x-axis.
 * @param y Coordinate on the y-axis.
 * @param width Width of cell to draw.
 * @param height Height of cell to draw.
 */
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

/**
 * @brief GraphicsCellItem::is_junction Returns true if the represented cell is a Junction object.
 */
bool GraphicsCellItem::is_junction()
{
    return(this->cell->is_junction());
}

/**
 * @brief GraphicsCellItem::has_vehicle Returns true if the represented cell contains a vehicle.
 */
bool GraphicsCellItem::has_vehicle()
{
    return(this->cell->has_vehicle());
}

/**
 * @brief GraphicsCellItem::get_vehicle Returns the Vehicle object of the represented cell.
 */
Vehicle *GraphicsCellItem::get_vehicle()
{
    return(this->cell->get_vehicle());
}

/**
 * @brief GraphicsCellItem::setSelected Selects the drawn cell in the GUI, thereby tracking its vehicle if any.
 */
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

/**
 * @brief GraphicsCellItem::vehicle_brush Returns a coloured QBrush for vehicle cells.
 */
QBrush GraphicsCellItem::vehicle_brush()
{
    return QBrush(Qt::green);
}

/**
 * @brief GraphicsCellItem::focused_vehicle_brush Returns a coloured QBrush for tracked vehicle cells.
 */
QBrush GraphicsCellItem::focused_vehicle_brush()
{
    return QBrush(Qt::red);
}

/**
 * @brief GraphicsCellItem::empty_cell_brush Returns an uncoloured QBrush for empty cells.
 */
QBrush GraphicsCellItem::empty_cell_brush()
{
    return QBrush(Qt::white);
}

/**
 * @brief qgraphicsitem_cast Allows casting of QGraphicsItem to GraphicsCellItem.
 */
GraphicsCellItem* qgraphicsitem_cast(QGraphicsItem *item)
{
    return (GraphicsCellItem*)item;
}
