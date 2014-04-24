#include "vehicle.h"

/**
 * @brief Vehicle::Vehicle Initialises a Vehicle object.
 * @param velocity Initial velocity.
 */
Vehicle::Vehicle(unsigned int velocity)
{
    this->velocity = velocity;
    this->generation = 0;
}

/**
 * @brief Vehicle::increment_velocity Increases the vehicle's velocity by one.
 */
void Vehicle::increment_velocity()
{
    if(this->velocity < this->MAXIMUM_VELOCITY)
        this->velocity++;
}

/**
 * @brief Vehicle::decrement_velocity Decreases the vehicle's velocity by one.
 */
void Vehicle::decrement_velocity()
{
    if(this->velocity > 0)
        this->velocity--;
}

/**
 * @brief Vehicle::decrease_velocity Decreases the vehicle's velocity to (steps - 1).
 */
void Vehicle::decrease_velocity(unsigned long steps)
{
    if(this->velocity > 0)
        this->velocity = steps - 1;
}

/**
 * @brief Vehicle::get_velocity Returns the vehicle's velocity.
 */
unsigned int Vehicle::get_velocity()
{
    return this->velocity;
}

/**
 * @brief Vehicle::get_maximum_velocity Returns the vehicle's maximum velocity.
 */
unsigned int Vehicle::get_maximum_velocity()
{
    return MAXIMUM_VELOCITY;
}

/**
 * @brief Vehicle::get_generation Returns the vehicle's generation.
 */
unsigned int Vehicle::get_generation()
{
    return this->generation;
}

/**
 * @brief Vehicle::increment_generation Increments the vehicle's generation by one.
 */
void Vehicle::increment_generation()
{
    this->generation++;
}

/**
 * @brief Vehicle::set_generation Redefines the vehicle's generation.
 */
void Vehicle::set_generation(unsigned long generation)
{
    this->generation = generation;
}
