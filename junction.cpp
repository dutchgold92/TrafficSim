#include "junction.h"

using namespace std;

Junction::Junction()
{
    this->generation = 0;
    this->active_incoming_road = 0;
}

bool Junction::is_junction()
{
    return true;
}

void Junction::connect_roads(Road *origin_road, Road *destination_road)
{
    if(!this->is_destination_road_connected(destination_road))
    {
        this->next_cells.push_back((Cell*)destination_road->get_first_cell());
        destination_road->get_first_cell()->set_previous_cell(this);
    }

    if(!this->is_origin_road_connected(origin_road))
    {
        this->previous_cells.push_back((Cell*)origin_road->get_last_cell());
        origin_road->get_last_cell()->set_next_cell(this);
    }
}

bool Junction::is_destination_road_connected(Road *destination_road)
{
    for(vector<Cell*>::size_type i = 0; i < this->next_cells.size(); i++)
    {
        if(this->next_cells.at(i) == destination_road->get_first_cell())
            return true;
    }

    return false;
}

bool Junction::is_origin_road_connected(Road *origin_road)
{
    for(vector<Cell*>::size_type i = 0; i < this->previous_cells.size(); i++)
    {
        if(this->previous_cells.at(i) == origin_road->get_last_cell())
            return true;
    }

    return false;
}

bool Junction::has_next_cell()
{
    return(this->next_cells.size() > 0);
}

bool Junction::has_previous_cell()
{
    return(this->previous_cells.size() > 0);
}

Cell* Junction::get_next_cell(Cell *origin_cell)
{
    unsigned long i = (rand() % this->next_cells.size() + 0); // FIXME: bad randomisation

    return this->next_cells.at(i);
}

vector<Cell*> Junction::get_next_cells()
{
    return this->next_cells;
}

Cell* Junction::get_previous_cell(Cell *origin_cell)
{
    unsigned long i = (rand() % this->previous_cells.size() + 0); // FIXME: bad randomisation

    return this->previous_cells.at(i);
}

vector<Cell*> Junction::get_previous_cells()
{
    return this->previous_cells;
}

bool Junction::is_accessible_from_road(Cell *cell)
{
    return(cell == this->active_incoming_road);
}

void Junction::toggle_active_incoming_road()
{
    if(this->active_incoming_road == 0)
        this->active_incoming_road = this->get_previous_cells().front();

    if(this->get_previous_cells().size() > 1)
    {
        if(this->active_incoming_road == this->previous_cells.back())
            this->active_incoming_road = this->previous_cells.front();
        else
        {
            for(vector<Cell*>::size_type i = 0; i < this->previous_cells.size(); i++)
            {
                if(this->active_incoming_road == this->previous_cells.at(i))
                {
                    this->active_incoming_road = this->previous_cells.at(i + 1);
                    return;
                }
            }
        }
    }
}
