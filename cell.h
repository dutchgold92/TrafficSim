#ifndef CELL_H
#define CELL_H

#include <vehicle.h>
#include <vector>

using namespace std;

class Cell
{
public:
    Cell();
    bool is_middle_cell();
    void set_middle_cell();
    enum display_direction {left_to_right, top_to_bottom, bottom_to_top, right_to_left};
    unsigned long get_generation();
    void increment_generation();
    unsigned long get_display_generation();
    void increment_display_generation();
    virtual bool is_junction();
    bool has_vehicle();
    void set_vehicle(Vehicle *vehicle);
    void reset_vehicle();
    Vehicle* get_vehicle();
    virtual bool has_next_cell();
    virtual bool has_previous_cell();
    void set_next_cell(Cell *cell);
    void set_previous_cell(Cell *cell);
    virtual Cell* get_next_cell(Cell *origin_cell);
    virtual Cell* get_previous_cell(Cell *origin_cell);
    void set_speed_limit(unsigned int speed_limit);
    unsigned int get_speed_limit();
    void set_direction(display_direction direction);
    display_direction get_direction();
private:
    bool middle_cell;
    unsigned long generation;
    unsigned long display_generation;
    Vehicle *vehicle;
    unsigned int speed_limit;
    Cell *next_cell;
    Cell *previous_cell;
    display_direction direction;
};

#endif // CELL_H
