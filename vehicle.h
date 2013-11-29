#ifndef VEHICLE_H
#define VEHICLE_H

class Vehicle
{
private:
    unsigned int velocity;
    const static unsigned int MAXIMUM_VELOCITY = 5;
    unsigned long generation;
public:
    Vehicle(unsigned int velocity = 0);
    void increment_velocity();
    void decrement_velocity();
    void decrease_velocity(unsigned long steps);
    unsigned int get_velocity();
    static unsigned int get_maximum_velocity();
    unsigned long __cell_index;
    unsigned long __road_id;
    bool __cell_is_junction;
    unsigned int get_generation();
    void increment_generation();
    void set_generation(unsigned long generation);
};

#endif // VEHICLE_H
