#include "cell.h"

using namespace std;

Cell::Cell()
{
    this->generation = 0;
    this->vehicle = 0;
    this->next_cell = 0;
    this->previous_cell = 0;
}

unsigned long Cell::get_generation()
{
    return this->generation;
}

void Cell::increment_generation()
{
    this->generation++;
}

unsigned long Cell::get_display_generation()
{
    return this->display_generation;
}

void Cell::increment_display_generation()
{
    this->display_generation++;
}

bool Cell::is_junction()
{
    return false;
}

bool Cell::has_vehicle()
{
    return (this->vehicle != 0);
}

void Cell::set_vehicle(Vehicle *vehicle)
{
    this->vehicle = vehicle;
}

void Cell::reset_vehicle()
{
    this->vehicle = 0;
}

Vehicle* Cell::get_vehicle()
{
    return this->vehicle;
}

bool Cell::has_next_cell()
{
    return(this->next_cell != 0);
}

bool Cell::has_previous_cell()
{
    return(this->previous_cell != 0);
}

void Cell::set_next_cell(Cell *cell)
{
    this->next_cell = cell;
}

void Cell::set_previous_cell(Cell *cell)
{
    this->previous_cell = cell;
}

Cell* Cell::get_next_cell(Cell *origin_cell)
{
    return this->next_cell;
}

Cell* Cell::get_previous_cell(Cell *origin_cell)
{
    return this->previous_cell;
}

void Cell::set_speed_limit(unsigned int speed_limit)
{
    this->speed_limit = speed_limit;
}

unsigned int Cell::get_speed_limit()
{
    return this->speed_limit;
}

void Cell::set_direction(display_direction direction)
{
    this->direction = direction;
}

Cell::display_direction Cell::get_direction()
{
    return this->direction;
}
