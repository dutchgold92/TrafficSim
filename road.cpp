#include "road.h"

Road::Road(unsigned long length)
{
    this->length = length;
    this->speed_limit = DEFAULT_SPEED_LIMIT;

    // initialise cells
    for(unsigned long i = 0; i < length; i++)
    {
        Cell *cell = new Cell();
        cell->set_speed_limit(this->speed_limit);
        this->cells.push_back(cell);
    }

    // link cells
    for(unsigned long i = 0; i < (this->cells.size() - 1); i++)
        ((Cell*)this->cells.at(i))->set_next_cell((Cell*)this->cells.at(i + 1));
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
