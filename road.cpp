#include "road.h"

Road::Road(unsigned long length, Cell::display_direction direction)
{
    this->length = length;
    this->direction = direction;
    this->speed_limit = DEFAULT_SPEED_LIMIT;

    // initialise cells
    for(unsigned long i = 0; i < length; i++)
    {
        Cell *cell = new Cell();
        cell->set_speed_limit(this->speed_limit);
        cell->set_direction(direction);

        if(i == (length / 2))
            cell->set_middle_cell();

        this->cells.push_back(cell);
    }

    // link cells forwards
    for(unsigned long i = 0; i < (this->cells.size() - 1); i++)
        ((Cell*)this->cells.at(i))->set_next_cell((Cell*)this->cells.at(i + 1));

    // link cells backwards
    for(unsigned long i = this->cells.size(); i-- > 1;)
        ((Cell*)this->cells.at(i))->set_previous_cell((Cell*)this->cells.at(i - 1));
}

unsigned long Road::get_length()
{
    return this->length;
}

void Road::append_cell(Cell *cell)
{
    this->length++;
    ((Cell*)this->cells.back())->set_next_cell(cell);
    this->cells.push_back(cell);
}

Cell* Road::get_cell(unsigned int index)
{
    if(index < this->cells.size())
        return this->cells.at(index);
}

Cell* Road::get_first_cell()
{
    return this->cells.front();
}

Cell* Road::get_last_cell()
{
    return this->cells.back();
}

float Road::get_density()
{
    float vehicle_count = 0;

    for(unsigned long i = 0; i < this->length; i++)
        if(this->get_cell(i)->has_vehicle())
            vehicle_count++;

    return (vehicle_count / float(this->length));
}

Cell::display_direction Road::get_direction()
{
    return this->direction;
}
