#ifndef JUNCTION_H
#define JUNCTION_H

#include <road.h>
#include <cell.h>
#include <stdlib.h>
#include <iostream>

class Junction : public Cell
{
private:
    vector<Cell*> next_cells;
    vector<Cell*> previous_cells;
public:
    Junction();
    bool is_junction();
    void connect_roads(Road *origin_road, Road *destination_road);
    bool has_next_cell();
    bool has_previous_cell();
    Cell* get_next_cell(Cell *origin);
};

#endif // JUNCTION_H
