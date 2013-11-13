#ifndef CELL_H
#define CELL_H

#include <vehicle.h>
#include <vector>

using namespace std;

class Cell
{
private:
    Vehicle *vehicle;
    unsigned int speed_limit;
    Cell *next_cell;
public:
    Cell();
    virtual bool is_junction();
    bool has_vehicle();
    void set_vehicle(Vehicle *vehicle);
    void reset_vehicle();
    Vehicle* get_vehicle();
    virtual bool has_next_cell();
    void set_next_cell(Cell *cell);
    virtual Cell* get_next_cell(Cell *origin_cell);
    void set_speed_limit(unsigned int speed_limit);
    unsigned int get_speed_limit();
};

#endif // CELL_H
