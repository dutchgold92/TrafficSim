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

#define DEFAULT_INITIAL_DENSITY 0.2

using namespace std;

class Network
{
private:
    unsigned long generation;
    vector<Road*> roads;
    vector<Road*> orphan_roads;
    vector<Road*> exit_roads;
    vector<Junction*> junctions;
    float desired_input_density;
    void init();
    void init_traffic();
    void identify_orphan_roads();
    void identify_exit_roads();
    void identify_junctions();
    bool is_known_junction(Junction *junction);
    void process();
    void process_road(Cell *first_cell, bool forward_processing);
    void process_cell(Cell *cell);
    void apply_acceleration(Cell *cell, Vehicle *vehicle);
    void apply_deceleration(Cell *cell, Vehicle *vehicle);
    void apply_randomisation(Vehicle *vehicle);
    void enter_junction(Junction *junction, Vehicle *vehicle);
    void apply_motion();
    void apply_motion_to_road(Cell *last_cell, bool forward_processing);
    void apply_motion_to_cell(Cell *cell);
    unsigned long get_front_clearance(Cell *cell);
    unsigned long get_distance_to_next_junction(Cell *cell);
    void synthesize_traffic();
public:
    Network(vector<Road*> roads);
    vector<Road*> get_roads();
    vector<Junction*> get_junctions();
    void step();
    void set_desired_input_density(float desired_input_density);
    float get_desired_input_density();
    float get_actual_input_density();
    float get_overall_density();
    vector<Road*> get_orphan_roads();
};

#endif // NETWORK_H
