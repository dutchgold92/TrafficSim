#include "network.h"

using namespace std;

Network::Network(vector<Road*> roads)
{
    this->generation = 0;
    this->desired_input_density = DEFAULT_INITIAL_DENSITY;
    this->roads = roads;
    init();
}

void Network::init()
{
    this->init_traffic();
    this->identify_orphan_roads();
    this->identify_exit_roads();
    this->identify_junctions();
}

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

void Network::identify_orphan_roads()
{
    for(vector<Road*>::iterator it = this->roads.begin(); it != this->roads.end(); ++it)
        if(!((Road*)*it)->get_first_cell()->has_previous_cell())
            this->orphan_roads.push_back((Road*)*it);
}

void Network::identify_exit_roads()
{
    for(vector<Road*>::reverse_iterator it = this->roads.rbegin(); it != this->roads.rend(); ++it)
        if(!((Road*)*it)->get_last_cell()->has_next_cell())
            this->exit_roads.push_back((Road*)*it);
}

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

vector<Road*> Network::get_roads()
{
    return this->roads;
}

vector<Junction*> Network::get_junctions()
{
    return this->junctions;
}

void Network::step()
{
    this->synthesize_traffic();
    this->process();
}

void Network::synthesize_traffic()
{
    //FIXME: debugging
    //cout << this->get_actual_input_density() << " / " << this->desired_input_density << endl;

    for(vector<Road*>::iterator it = this->orphan_roads.begin(); (it != this->orphan_roads.end()) && (this->get_actual_input_density() <= this->get_desired_input_density()); ++it)
    {
        Cell *cell = ((Road*)*it)->get_first_cell();

        if(!cell->has_vehicle())
            cell->set_vehicle(new Vehicle(Vehicle::get_maximum_velocity() / 2));
    }
}

void Network::process()
{
    for(vector<Road*>::iterator it = this->orphan_roads.begin(); it != this->orphan_roads.end(); ++it)
        this->process_road(((Road*)*it)->get_first_cell());

    this->generation++;
    apply_motion();
    this->generation++;
}

void Network::process_road(Cell *first_cell)
{
    Cell *cell = first_cell;

    for(;;)
    {
        if(cell->get_generation() <= this->generation)
            this->process_cell(cell);

        if(cell->is_junction())
        {
            Junction *junction = (Junction*)cell;

            for(unsigned long i = 0; i < junction->get_next_cells().size(); i++)
            {
                Cell *next_cell = junction->get_next_cells().at(i);

                if(next_cell->get_generation() <= this->generation)
                    this->process_road(next_cell);
            }

            return;
        }

        if(cell->has_next_cell())
            cell = cell->get_next_cell(cell);
        else
            return;
    }
}

void Network::process_cell(Cell *cell)
{
    if(cell->has_vehicle())
    {
        Vehicle *vehicle = cell->get_vehicle();
        apply_acceleration(cell, vehicle);
        apply_deceleration(cell, vehicle);
        apply_randomisation(vehicle);
    }

    cell->increment_generation();
}

void Network::apply_acceleration(Cell *cell, Vehicle *vehicle)
{
    if(vehicle->get_velocity() < vehicle->get_maximum_velocity())
    {
        if(get_front_clearance(cell) > (vehicle->get_velocity() + 1))
            vehicle->increment_velocity();
    }
}

void Network::apply_deceleration(Cell *cell, Vehicle *vehicle)
{
    if(vehicle->get_velocity() > 0)
    {
        unsigned long distance_to_junction = get_distance_to_next_junction(cell);

        if((vehicle->get_velocity() > 2) && (distance_to_junction <= vehicle->get_velocity()))
            vehicle->decrease_velocity(distance_to_junction + 1);

        unsigned long front_clearance = get_front_clearance(cell);

        if(front_clearance <= vehicle->get_velocity())
            vehicle->decrease_velocity(front_clearance);
    }
}

void Network::apply_randomisation(Vehicle *vehicle)
{
    if((rand() % 5 + 0) == 4)   // FIXME: bad randomisation
        vehicle->decrement_velocity();
}

void Network::apply_motion()
{
    for(vector<Road*>::reverse_iterator it = this->exit_roads.rbegin(); it != this->exit_roads.rend(); ++it)
        this->apply_motion_to_road(((Road*)*it)->get_last_cell());
}

void Network::apply_motion_to_road(Cell *last_cell)
{
    Cell *cell = last_cell;

    for(;;)
    {
        if(cell->get_generation() <= this->generation)
            this->apply_motion_to_cell(cell);

        if(cell->is_junction())
        {
            Junction *junction = (Junction*)cell;

            for(unsigned long i = junction->get_previous_cells().size(); i-- > 0;)
            {
                Cell *previous_cell = junction->get_previous_cells().at(i);

                if(previous_cell->get_generation() <= this->generation)
                    this->apply_motion_to_road(previous_cell);
            }

            return;
        }

        if(cell->has_previous_cell())
            cell = cell->get_previous_cell(cell);
        else
            return;
    }
}

void Network::apply_motion_to_cell(Cell *cell)
{
    if(cell->has_vehicle())
    {
        Vehicle *v = cell->get_vehicle();
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
                    break;
                }
            }
        }
    }
}

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

void Network::set_desired_input_density(float desired_input_density)
{
    this->desired_input_density = desired_input_density;
}

float Network::get_desired_input_density()
{
    return this->desired_input_density;
}

float Network::get_actual_input_density()
{
    float vehicles = 0;
    float cells = 0;

    for(vector<Road*>::iterator it = this->orphan_roads.begin(); it != this->orphan_roads.end(); ++it)
    {
        for(unsigned long i = 0; i < ((Road*)*it)->get_length(); i++)
        {
            if(((Cell*)((Road*)*it)->get_cell(i))->has_vehicle())
                vehicles++;

            cells++;
        }
    }

    return(vehicles / cells);
}

float Network::get_overall_density()
{
    float roads = this->roads.size();
    float densities = 0;

    for(unsigned int i = 0; i < this->roads.size(); i++)
        densities += ((Road*)this->roads.at(i))->get_density();

    return (densities / roads);
}

vector<Road*> Network::get_orphan_roads()
{
    return this->orphan_roads;
}
