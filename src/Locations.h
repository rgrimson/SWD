#pragma once

#include "ofMain.h"
#include "Solver2.h"
#include "Graph.h"
#include "Parameters.h"


using namespace std;

/********************/
/*** Locations *****/
/********************/
class locations_t
{
    /** VARIABLES **/
    string *name;
    FILE *out_file;
    idx_int n_locations;
    param_t *param;
    solver_t *solver;
    ofVec2f *pos; //graphical pos

public:
    idx_int *river;
    double *pos_in_river;
    ~locations_t();
    void load(param_t *param_in, solver_t *solver_in, graph_t *graph);
    void open_output_file();
    void save_step();
    void draw();
};
//]
