#include "network.h"

Network::Network(vector<Road *> roads)
{
    this->desired_density = DEFAULT_DESIRED_DENSITY;
    this->roads = roads;
    init();
}

void Network::init()
{
    this->synthesize_traffic();
}

vector<Road*> Network::get_roads()
{
    return this->roads;
}

void Network::step()
{
    this->synthesize_traffic();
    this->process();
}

void Network::synthesize_traffic()
{
    cout << this->get_overall_density() << " / " << this->desired_density << endl;
    if(this->get_overall_density() < this->desired_density)
    {
        Cell *cell = ((Road*)this->roads.front())->get_first_cell();

        do
        {
            if(this->get_overall_density() >= this->desired_density)
                return;
            else if(!cell->has_vehicle())
            {
                cell->set_vehicle(new Vehicle(Vehicle::get_maximum_velocity() / 2));
                cell = cell->get_next_cell(cell);
            }
        }
        while(cell->has_next_cell() && !cell->has_vehicle());
    }
}

void Network::process()
{
    for(unsigned long i = this->roads.size(); i-- > 0;)
    {
        Road *road = this->roads.at(i);

        for(unsigned long x = road->get_length(); x-- > 0;)
        {
            Cell *cell = road->get_cell(x);

            if(cell->has_vehicle())
            {
                Vehicle *vehicle = cell->get_vehicle();
                apply_acceleration(cell, vehicle);
                apply_deceleration(cell, vehicle);
                //apply_randomisation(vehicle);
            }
        }
    }

    apply_motion();
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
    vector<Road*> n = this->get_roads();

    for(unsigned long i = n.size(); i-- > 0;)
    {
        Road *r = n.at(i);

        for(unsigned long x = r->get_length(); x-- > 0;)
        {
            Cell *c = r->get_cell(x);

            if(c->has_vehicle())
            {
                Vehicle *v = c->get_vehicle();
                unsigned int cells_to_move = v->get_velocity();

                if(cells_to_move > 0)
                {
                    c->reset_vehicle();

                    while(c->has_next_cell())
                    {
                        c = c->get_next_cell(c);
                        cells_to_move--;

                        if(cells_to_move == 0)
                        {
                            c->set_vehicle(v);
                            break;
                        }
                    }
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

void Network::set_desired_density(float desired_density)
{
    this->desired_density = desired_density;
}

float Network::get_desired_density()
{
    return this->desired_density;
}

float Network::get_overall_density()
{
    float roads = this->roads.size();
    float densities = 0;

    for(unsigned int i = 0; i < this->roads.size(); i++)
        densities += ((Road*)this->roads.at(i))->get_density();

    return (densities / roads);
}
