#include "cell.h"

using namespace std;

Cell::Cell()
{
    this->vehicle = 0;
    this->next_cell = 0;
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
    return (this->next_cell != 0);
}

void Cell::set_next_cell(Cell *cell)
{
    this->next_cell = cell;
}

Cell* Cell::get_next_cell(Cell *origin_cell)
{
    return this->next_cell;
}

void Cell::set_speed_limit(unsigned int speed_limit)
{
    this->speed_limit = speed_limit;
}

unsigned int Cell::get_speed_limit()
{
    return this->speed_limit;
}