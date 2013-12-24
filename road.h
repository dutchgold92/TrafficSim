#ifndef ROAD_H
#define ROAD_H

#include <cell.h>

#define DEFAULT_SPEED_LIMIT 5

class Road
{
public:
    Road(unsigned long length, Cell::display_direction direction);
    unsigned long get_length();
    void append_cell(Cell* cell);
    Cell* get_cell(unsigned int index);
    Cell* get_first_cell();
    Cell* get_last_cell();
    float get_density();
    Cell::display_direction get_direction();
    unsigned int get_speed_limit();
private:
    unsigned long length;
    unsigned int speed_limit;
    vector<Cell*> cells;
    Cell::display_direction direction;
};

#endif // ROAD_H
