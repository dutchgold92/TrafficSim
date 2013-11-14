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

#define DEFAULT_INITIAL_DENSITY 0.6

using namespace std;

class Network
{
private:
    vector<Road*> roads;
    vector<Road**> orphan_roads;
    float desired_input_density;
    void init();
    void init_traffic();
    void identify_orphan_roads();
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
    void set_desired_input_density(float desired_input_density);
    float get_desired_input_density();
    float get_actual_input_density();
    float get_overall_density();
};

#endif // NETWORK_H
