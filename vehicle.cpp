#include "vehicle.h"

Vehicle::Vehicle(unsigned int velocity)
{
    this->velocity = velocity;
    this->generation = 0;
}

void Vehicle::increment_velocity()
{
    if(this->velocity < this->MAXIMUM_VELOCITY)
        this->velocity++;
}

void Vehicle::decrement_velocity()
{
    if(this->velocity > 0)
        this->velocity--;
}

void Vehicle::decrease_velocity(unsigned long steps)
{
    if(this->velocity > 0)
        this->velocity = steps - 1;
}

unsigned int Vehicle::get_velocity()
{
    return this->velocity;
}

unsigned int Vehicle::get_maximum_velocity()
{
    return MAXIMUM_VELOCITY;
}

unsigned int Vehicle::get_generation()
{
    return this->generation;
}

void Vehicle::increment_generation()
{
    this->generation++;
}
