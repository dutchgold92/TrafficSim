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
    bool is_destination_road_connected(Road *destination_road);
    bool is_origin_road_connected(Road *origin_road);
public:
    Junction();
    bool is_junction();
    void connect_roads(Road *origin_road, Road *destination_road);
    bool has_next_cell();
    bool has_previous_cell();
    Cell* get_next_cell(Cell *origin_cell);
    vector<Cell*> get_next_cells();
    Cell* get_previous_cell(Cell *origin_cell);
    vector<Cell*> get_previous_cells();
};

#endif // JUNCTION_H
