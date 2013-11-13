#include "junction.h"

using namespace std;

Junction::Junction()
{

}

bool Junction::is_junction()
{
    return true;
}

void Junction::connect_roads(Road *origin_road, Road *destination_road)
{
    this->next_cells.push_back(destination_road->get_first_cell());
    origin_road->append_cell(this);
}

bool Junction::has_next_cell()
{
    return(this->next_cells.size() > 0);
}

Cell* Junction::get_next_cell(Cell *origin)
{
    unsigned long i = (rand() % this->next_cells.size() + 0); // FIXME: bad randomisation

    return this->next_cells.at(i);
}
