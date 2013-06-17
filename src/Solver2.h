#pragma once

#include <armadillo>
#include "Graph.h"
#include "Parameters.h"

//#DEFINE theta (param->theta)
//#DEFINE g (param->g)
//#DEFINE cm (param->cm)


using namespace arma;


class solver_t
{
private:
    graph_t *graph;
    param_t *param;
    idx_int dimx, n_int_variables;
    mat A;
    mat b;
    mat w;
    vector <idx_int> w_idx;
    void define_w_index(graph_t *graph);

public:
    void setup(graph_t *graph_in, param_t *param_in); //definir las variables A, b y w, el indice y escribir en A las condiciones fijas (kirchoff), escribir el w inicial a partir de hq
    void solve(double *cboundary); //define_A_b(&A,&b), solve

    void qh(idx_int river, double pos_in_river,double * buffer);
    void set_w_constant(double h, graph_t *graph);
    void hq2w(graph_t *graph);
    void w2hq(graph_t *graph);
    void load_w();
    void save_w();
    void save_Ab();
};

