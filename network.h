#ifndef NETWORK_H
#define NETWORK_H

#include <road.h>
#include <cell.h>
#include <junction.h>
#include <vehicle.h>
#include <vector>
#include <limits>
#include <iostream>
#include <stdlib.h>

#define DEFAULT_DESIRED_DENSITY 0.2

using namespace std;

class Network
{
private:
    vector<Road*> roads;
    float desired_density;
    void init();
    void process();
    void apply_acceleration(Cell *cell, Vehicle *vehicle);
    void apply_deceleration(Cell *cell, Vehicle *vehicle);
    void apply_randomisation(Vehicle *vehicle);
    void apply_motion();
    unsigned long get_front_clearance(Cell *cell);
    unsigned long get_distance_to_next_junction(Cell *cell);
    void synthesize_traffic();
public:
    Network(vector<Road*> roads);
    vector<Road*> get_roads();
    void step();
    void set_desired_density(float desired_density);
    float get_desired_density();
    float get_overall_density();
};

#endif // NETWORK_H
