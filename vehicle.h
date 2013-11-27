#ifndef VEHICLE_H
#define VEHICLE_H

class Vehicle
{
private:
    unsigned int velocity;
    const static unsigned int MAXIMUM_VELOCITY = 5;
public:
    Vehicle(unsigned int velocity = 0);
    void increment_velocity();
    void decrement_velocity();
    void decrease_velocity(unsigned long steps);
    unsigned int get_velocity();
    static unsigned int get_maximum_velocity();
    signed long __cell_index = -1;
};

#endif // VEHICLE_H
