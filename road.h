#ifndef ROAD_H
#define ROAD_H

#include <cell.h>

#define DEFAULT_SPEED_LIMIT 4

class Road
{
private:
    unsigned long length;
    unsigned int speed_limit;
    vector<Cell*> cells;
public:
    Road(unsigned long length);
    unsigned long get_length();
    void append_cell(Cell* cell);
    Cell* get_cell(unsigned int index);
    Cell* get_first_cell();
    Cell* get_last_cell();
    float get_density();
};

#endif // ROAD_H
