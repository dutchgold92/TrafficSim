#include "network.h"

using namespace std;

/**
 * @brief Network::Network Basic initialisation of the Network object.
 * @param roads Roads to use for the model.
 */
Network::Network(vector<Road*> roads)
{
    this->generation = 0;
    this->desired_input_density = DEFAULT_INITIAL_DENSITY;
    this->roads = roads;
    init();
}

/**
 * @brief Network::init Further initialisation of the Network object.
 */
void Network::init()
{
    this->init_traffic();
    this->identify_input_roads();
    this->identify_output_roads();
    this->identify_junctions();
}

/**
 * @brief Network::init_traffic Initialises traffic in the model.
 */
void Network::init_traffic()
{
    while(this->get_overall_density() < this->get_desired_input_density())
    {
        for(vector<Road*>::iterator it = this->roads.begin(); (it != this->roads.end()) && (this->get_overall_density() <= this->get_desired_input_density()); ++it)
        {
            Cell *cell = ((Road*)*it)->get_first_cell();

            while(((Road*)*it)->get_density() < this->get_desired_input_density())
            {
                if((!cell->has_vehicle()) && ((rand() % 3 + 0) == 0))
                    cell->set_vehicle(new Vehicle());

                if(cell->has_next_cell())
                    cell = cell->get_next_cell(cell);
                else
                    cell = ((Road*)*it)->get_first_cell();
            }
        }
    }
}

/**
 * @brief Network::identify_input_roads Detects input roads.
 */
void Network::identify_input_roads()
{
    for(vector<Road*>::iterator it = this->roads.begin(); it != this->roads.end(); ++it)
        if(!((Road*)*it)->get_first_cell()->has_previous_cell())
            this->input_roads.push_back((Road*)*it);
}

/**
 * @brief Network::identify_output_roads Detects output roads.
 */
void Network::identify_output_roads()
{
    for(vector<Road*>::reverse_iterator it = this->roads.rbegin(); it != this->roads.rend(); ++it)
        if(!((Road*)*it)->get_last_cell()->has_next_cell())
            this->output_roads.push_back((Road*)*it);
}

/**
 * @brief Network::identify_junctions Detects cells which are Junction objects.
 */
void Network::identify_junctions()
{
    for(vector<Road*>::iterator it = this->roads.begin(); it != this->roads.end(); ++it)
    {
        Cell *cell = ((Road*)*it)->get_first_cell();

        while(cell->has_next_cell())
        {
            if(cell->is_junction() && !this->is_known_junction((Junction*)cell))
            {
                this->junctions.push_back((Junction*)cell);
            }

            cell = cell->get_next_cell(cell);
        }
    }
}

/**
 * @brief Network::is_known_junction Returns true if a Junction object has already been learned by identify_junctions().
 * @param junction Junction to find.
 */
bool Network::is_known_junction(Junction *junction)
{
    if(this->junctions.empty())
        return false;

    for(vector<Junction*>::iterator it = this->junctions.begin(); it != this->junctions.end(); ++it)
    {
        if(junction == (Junction*)*it)
            return true;
    }

    return false;
}

/**
 * @brief Network::get_roads Returns a vector of all Road objects in the Network.
 */
vector<Road*> Network::get_roads()
{
    return this->roads;
}

/**
 * @brief Network::get_junctions Returns a vector of all Junction objects in the Network.
 */
vector<Junction*> Network::get_junctions()
{
    return this->junctions;
}

/**
 * @brief Network::step Orders a single evolution of the model.
 */
void Network::step()
{
    this->process();
    this->synthesize_traffic();
}

/**
 * @brief Network::synthesize_traffic Creates new traffic in the model, attempting to meet density requirements.
 */
void Network::synthesize_traffic()
{
    vector<vector<Cell*> > cells;

    for(vector<Road*>::size_type i = 0; i < this->input_roads.size(); i++)
    {
        Road *r = this->input_roads.at(i);
        vector<Cell*> v;

        for(vector<Cell*>::size_type x = 0; x < r->get_length() && x < r->get_speed_limit(); x++)
        {
            Cell *c = r->get_cell(x);

            if(!c->has_vehicle())
                v.push_back(c);
            else
                break;
        }

        if(!v.empty())
            cells.push_back(v);
    }

    while((this->get_actual_input_density() < this->get_desired_input_density()) && !cells.empty())
    {
        unsigned int index = (rand() % cells.size() + 0);

        if(!cells.at(index).empty())
        {
            Cell *cell = cells.at(index).back();

            Vehicle *v = new Vehicle(cell->get_speed_limit());
            v->set_generation(this->generation);
            cell->set_vehicle(v);

            cells.at(index).pop_back();
        }
        else
            cells.erase(cells.begin() + index);
    }
}

/**
 * @brief Network::process Processes a single model evolution, applying rules.
 */
void Network::process()
{
    this->generation++;
    this->process_road(((Road*)this->roads.back())->get_last_cell(), false);
    this->generation++;
    this->apply_motion_to_road(((Road*)this->roads.back())->get_last_cell(), false);

    for(unsigned int i = 0; i < this->junctions.size(); i++)
        ((Junction*)this->junctions.at(i))->toggle_active_incoming_road();
}

/**
 * @brief Network::process_road Processes a single model road, applying rules to its cells.
 * @param first_cell Cell at which to start.
 * @param forward_processing Direction of processing.
 */
void Network::process_road(Cell *first_cell, bool forward_processing)
{
    Cell *cell = first_cell;

    for(;;)
    {
        if(cell->get_generation() < this->generation)
            this->process_cell(cell);

        if(cell->is_junction())
        {
            Junction *junction = (Junction*)cell;

            for(unsigned long i = 0; i < junction->get_next_cells().size(); i++)
            {
                Cell *next_cell = junction->get_next_cells().at(i);

                if(next_cell->get_generation() < this->generation)
                    this->process_road(next_cell, true);
            }

            for(unsigned long i = 0; i < junction->get_previous_cells().size(); i++)
            {
                Cell *previous_cell = junction->get_previous_cells().at(i);

                if(previous_cell->get_generation() < this->generation)
                    this->process_road(previous_cell, false);
            }

            return;
        }

        if(forward_processing && cell->has_next_cell())
            cell = cell->get_next_cell(cell);
        else if(!forward_processing && cell->has_previous_cell())
            cell = cell->get_previous_cell(cell);
        else
            return;
    }
}

/**
 * @brief Network::process_cell Processes a single model cell, applying rules to it.
 * @param cell Cell to manipulate.
 */
void Network::process_cell(Cell *cell)
{
    if(cell->has_vehicle() && cell->get_vehicle()->get_generation() < this->generation)
    {
        Vehicle *vehicle = cell->get_vehicle();
        apply_acceleration(cell, vehicle);
        apply_deceleration(cell, vehicle);
        apply_randomisation(vehicle);

        vehicle->increment_generation();
    }

    cell->increment_generation();
}

/**
 * @brief Network::apply_acceleration Applies the acceleration rule to a cell.
 * @param cell Cell to manipulate.
 * @param vehicle Vehicle to manipulate.
 */
void Network::apply_acceleration(Cell *cell, Vehicle *vehicle)
{
    if(vehicle->get_velocity() < vehicle->get_maximum_velocity())
    {
        if(get_front_clearance(cell) > (vehicle->get_velocity() + 1))
            vehicle->increment_velocity();
    }
}

/**
 * @brief Network::apply_deceleration Applies the deceleration rule to a cell.
 * @param cell Cell to manipulate.
 * @param vehicle Vehicle to manipulate.
 */
void Network::apply_deceleration(Cell *cell, Vehicle *vehicle)
{
    if(vehicle->get_velocity() > 0)
    {
        unsigned long distance_to_junction = get_distance_to_next_junction(cell);

        if(distance_to_junction <= vehicle->get_velocity())
        {
            Cell *junction;
            Cell *cell_before_junction = cell;

            for(unsigned long i = 0; i < (distance_to_junction - 1); i++)
                cell_before_junction = cell_before_junction->get_next_cell(cell_before_junction);

            junction = cell_before_junction->get_next_cell(cell_before_junction);

            if(!((Junction*)junction)->is_accessible_from_road(cell_before_junction))
                vehicle->decrease_velocity(distance_to_junction);
            else
                vehicle->decrease_velocity(distance_to_junction + 1);
        }

        unsigned long front_clearance = get_front_clearance(cell);

        if(front_clearance <= vehicle->get_velocity())
            vehicle->decrease_velocity(front_clearance);
    }
}

/**
 * @brief Network::apply_randomisation Applies the randomisation rule to a cell's vehicle.
 * @param vehicle Vehicle to randomise.
 */
void Network::apply_randomisation(Vehicle *vehicle)
{
    if((rand() % 5 + 0) == 4)
        vehicle->decrement_velocity();
}

/**
 * @brief Network::apply_motion_to_road Applies the progress rule to a road, cell by cell.
 * @param last_cell Cell at which to end.
 * @param forward_processing Direction of processing.
 */
void Network::apply_motion_to_road(Cell *last_cell, bool forward_processing)
{
    Cell *cell = last_cell;

    for(;;)
    {
        if(cell->get_generation() < this->generation)
            this->apply_motion_to_cell(cell);

        if(cell->is_junction())
        {
            Junction *junction = (Junction*)cell;

            for(vector<Cell*>::size_type i = junction->get_next_cells().size(); i-- > 0;)
            {
                Cell *next_cell = junction->get_next_cells().at(i);

                if(next_cell->get_generation() < this->generation)
                    this->apply_motion_to_road(next_cell, true);
            }

            for(vector<Cell*>::size_type i = junction->get_previous_cells().size(); i-- > 0;)
            {
                Cell *previous_cell = junction->get_previous_cells().at(i);

                if(previous_cell->get_generation() < this->generation)
                    this->apply_motion_to_road(previous_cell, false);
            }

            return;
        }
        else
        {
            if(forward_processing && cell->has_next_cell())
                cell = cell->get_next_cell(cell);
            else if(!forward_processing && cell->has_previous_cell())
                cell = cell->get_previous_cell(cell);
            else
                return;
        }
    }
}

/**
 * @brief Network::apply_motion_to_cell Applies the progress rule to a single cell.
 * @param cell Cell to process.
 */
void Network::apply_motion_to_cell(Cell *cell)
{
    cell->increment_generation();

    if(cell->has_vehicle() && cell->get_vehicle()->get_generation() < this->generation)
    {
        Vehicle *v = cell->get_vehicle();
        v->increment_generation();
        unsigned int cells_to_move = v->get_velocity();

        if(cells_to_move > 0)
        {
            cell->reset_vehicle();

            while(cell->has_next_cell())
            {
                cell = cell->get_next_cell(cell);
                cells_to_move--;

                if(cells_to_move == 0)
                {
                    cell->set_vehicle(v);
                    return;
                }
            }
        }
    }
}

/**
 * @brief Network::get_front_clearance Returns the front vehicle clearance from a particular cell.
 * @param cell Cell for which to calculate clearance.
 */
unsigned long Network::get_front_clearance(Cell *cell)
{
    unsigned long distance = 0;

    while(cell->has_next_cell())
    {
        cell = cell->get_next_cell(cell);
        distance++;

        if(cell->has_vehicle())
            return distance;
    }

    return numeric_limits<unsigned long int>::max();
}

/**
 * @brief Network::get_distance_to_next_junction Returns the vehicle clearance to next Junction.
 * @param cell Cell from which to begin.
 */
unsigned long Network::get_distance_to_next_junction(Cell *cell)
{
    unsigned long distance = 0;

    while(cell->has_next_cell())
    {
        cell = cell->get_next_cell(cell);
        distance++;

        if(cell->is_junction())
            return distance;
    }

    return numeric_limits<unsigned long int>::max();
}

/**
 * @brief Network::set_desired_input_density Set the model's optimal input density value.
 */
void Network::set_desired_input_density(float desired_input_density)
{
    this->desired_input_density = desired_input_density;
}

/**
 * @brief Network::get_desired_input_density Returns the model's optimal input density value.
 */
float Network::get_desired_input_density()
{
    return this->desired_input_density;
}

/**
 * @brief Network::get_actual_input_density Returns the model's actual input density.
 */
float Network::get_actual_input_density()
{
    float densities = 0;
    unsigned int roads = this->input_roads.size();

    for(vector<Road*>::size_type i = 0; i < roads; i++)
        densities += ((Road*)this->input_roads.at(i))->get_density();

    return(densities / (float)roads);
}

/**
 * @brief Network::get_overall_density Returns the model's actual overall density.
 */
float Network::get_overall_density()
{
    float roads = this->roads.size();
    float densities = 0;

    for(unsigned int i = 0; i < this->roads.size(); i++)
        densities += ((Road*)this->roads.at(i))->get_density();

    return (densities / roads);
}

/**
 * @brief Network::get_input_roads Returns a vector of the model's input Road objects.
 */
vector<Road*> Network::get_input_roads()
{
    return this->input_roads;
}
