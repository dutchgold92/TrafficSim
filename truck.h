#ifndef TRUCK_H
#define TRUCK_H

#include <vehicle.h>

class Truck : public Vehicle
{
private:
    const unsigned int MAXIMUM_VELOCITY = 4;
public:
    Truck();
};

#endif // TRUCK_H
