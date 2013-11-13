#ifndef CAR_H
#define CAR_H

#include <vehicle.h>

class Car : public Vehicle
{
private:
    const unsigned int MAXIMUM_VELOCITY = 5;
public:
    Car();
};

#endif // CAR_H
