#include "junction.h"

using namespace std;

/**
 * @brief Junction::Junction Initialises the Junction object.
 */
Junction::Junction()
{
    this->generation = 0;
    this->active_incoming_road = 0;
}

/**
 * @brief Junction::is_junction Returns true, to indicate that the cell is a Junction.
 */
bool Junction::is_junction()
{
    return true;
}

/**
 * @brief Junction::connect_roads Connects the given roads, by setting their next and previous cells to each other.
 * @param origin_road Input road.
 * @param destination_road Output road.
 */
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

/**
 * @brief Junction::is_destination_road_connected Returns true if the given output road is connected to this Junction.
 */
bool Junction::is_destination_road_connected(Road *destination_road)
{
    for(vector<Cell*>::size_type i = 0; i < this->next_cells.size(); i++)
    {
        if(this->next_cells.at(i) == destination_road->get_first_cell())
            return true;
    }

    return false;
}

/**
 * @brief Junction::is_origin_road_connected Returns true if the given input road is connected to this junction.
 */
bool Junction::is_origin_road_connected(Road *origin_road)
{
    for(vector<Cell*>::size_type i = 0; i < this->previous_cells.size(); i++)
    {
        if(this->previous_cells.at(i) == origin_road->get_last_cell())
            return true;
    }

    return false;
}

/**
 * @brief Junction::has_next_cell Returns true if the Junction has at least one next cell.
 */
bool Junction::has_next_cell()
{
    return(this->next_cells.size() > 0);
}

/**
 * @brief Junction::has_previous_cell Returns true if the Junction has at least one previous cell.
 */
bool Junction::has_previous_cell()
{
    return(this->previous_cells.size() > 0);
}

/**
 * @brief Junction::get_next_cell Returns a next cell at random.
 * @param origin_cell Origin of the traffic that needs a next-cell.
 */
Cell* Junction::get_next_cell(Cell *origin_cell)
{
    unsigned long i = (rand() % this->next_cells.size() + 0);

    return this->next_cells.at(i);
}

/**
 * @brief Junction::get_next_cells Returns all next cells as a vector.
 */
vector<Cell*> Junction::get_next_cells()
{
    return this->next_cells;
}

/**
 * @brief Junction::get_previous_cell Returns a previous cell at random.
 * @param origin_cell Origin of the traffic that needs a previous-cell.
 */
Cell* Junction::get_previous_cell(Cell *origin_cell)
{
    unsigned long i = (rand() % this->previous_cells.size() + 0); // FIXME: bad randomisation

    return this->previous_cells.at(i);
}

/**
 * @brief Junction::get_previous_cells Returns all previous cells as a vector.
 */
vector<Cell*> Junction::get_previous_cells()
{
    return this->previous_cells;
}

/**
 * @brief Junction::is_accessible_from_road Returns true if the Junction is accessible from a given cell.
 * @param cell Cell to test.
 */
bool Junction::is_accessible_from_road(Cell *cell)
{
    return(cell == this->active_incoming_road);
}

/**
 * @brief Junction::toggle_active_incoming_road Toggles the active incoming road connection.
 */
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
