#include "cell.h"

using namespace std;

/**
 * @brief Cell::Cell Initialises a Cell object.
 */
Cell::Cell()
{
    this->middle_cell = false;
    this->generation = 0;
    this->display_generation = 0;
    this->vehicle = 0;
    this->next_cell = 0;
    this->previous_cell = 0;
}

/**
 * @brief Cell::is_middle_cell Returns true if the cell is in a middle position in its parent road.
 */
bool Cell::is_middle_cell()
{
    return this->middle_cell;
}

/**
 * @brief Cell::set_middle_cell Specifies that the cell is in a middle position in its parent road.
 */
void Cell::set_middle_cell()
{
    this->middle_cell = true;
}

/**
 * @brief Cell::get_generation Returns the current generation of the cell.
 */
unsigned long Cell::get_generation()
{
    return this->generation;
}

/**
 * @brief Cell::increment_generation Increases the cell's current generation by one.
 */
void Cell::increment_generation()
{
    this->generation++;
}

/**
 * @brief Cell::get_display_generation Returns the latest cell generation which has been displayed in the GUI.
 */
unsigned long Cell::get_display_generation()
{
    return this->display_generation;
}

/**
 * @brief Cell::increment_display_generation Increases the cell's current display generation by one.
 */
void Cell::increment_display_generation()
{
    this->display_generation++;
}

/**
 * @brief Cell::is_junction Returns false to indicate the cell is not a Junction object.
 */
bool Cell::is_junction()
{
    return false;
}

/**
 * @brief Cell::has_vehicle Returns true if the cell contains a vehicle.
 */
bool Cell::has_vehicle()
{
    return (this->vehicle != 0);
}

/**
 * @brief Cell::set_vehicle Adds the given vehicle to the cell.
 */
void Cell::set_vehicle(Vehicle *vehicle)
{
    this->vehicle = vehicle;
}

/**
 * @brief Cell::reset_vehicle Dereferences any vehicles from the cell.
 */
void Cell::reset_vehicle()
{
    this->vehicle = 0;
}

/**
 * @brief Cell::get_vehicle Returns the cell's current vehicle.
 */
Vehicle* Cell::get_vehicle()
{
    return this->vehicle;
}

/**
 * @brief Cell::has_next_cell Returns true if the cell has an immediately following cell.
 */
bool Cell::has_next_cell()
{
    return(this->next_cell != 0);
}

/**
 * @brief Cell::has_previous_cell Returns true if the cell immediately follows another cell.
 */
bool Cell::has_previous_cell()
{
    return(this->previous_cell != 0);
}

/**
 * @brief Cell::set_next_cell Sets the next cell of this cell.
 */
void Cell::set_next_cell(Cell *cell)
{
    this->next_cell = cell;
}

/**
 * @brief Cell::set_previous_cell Sets the previous cell of this cell.
 */
void Cell::set_previous_cell(Cell *cell)
{
    this->previous_cell = cell;
}

/**
 * @brief Cell::get_next_cell Returns the next cell of this cell.
 * @param origin_cell Unused in this class.
 */
Cell* Cell::get_next_cell(Cell *origin_cell)
{
    return this->next_cell;
}

/**
 * @brief Cell::get_previous_cell Returns the previous cell of this cell.
 * @param origin_cell Unused in this class.
 */
Cell* Cell::get_previous_cell(Cell *origin_cell)
{
    return this->previous_cell;
}

/**
 * @brief Cell::set_speed_limit Sets the cell's velocity limit.
 */
void Cell::set_speed_limit(unsigned int speed_limit)
{
    this->speed_limit = speed_limit;
}

/**
 * @brief Cell::get_speed_limit Returns the cell's velocity limit.
 */
unsigned int Cell::get_speed_limit()
{
    return this->speed_limit;
}

/**
 * @brief Cell::set_direction Sets the direction of this cell.
 */
void Cell::set_direction(display_direction direction)
{
    this->direction = direction;
}

/**
 * @brief Cell::get_direction Returns the direction of this cell.
 */
Cell::display_direction Cell::get_direction()
{
    return this->direction;
}
