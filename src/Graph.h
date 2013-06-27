#pragma once

#include <vector>
#include "ofMain.h"
#include "Parameters.h"
using namespace std;

// type for idxex of rivers or nodes or small? things
typedef unsigned int idx_int;
typedef vector<idx_int> vec_idx;
typedef vector<double> vec_double;
typedef vector<bool> vec_bool;

// nodos
class node_t
{
public:
    double height;
	vec_idx river_in, river_out;
	idx_int degree;
	ofVec2f pos;
};
//]

//[ Rios
class river_t
{
public:
	string name;
    idx_int node_start, node_end;
    idx_int section;
    double length;
    double dx;

    idx_int n_discret_pts; //total
    idx_int n_int_discret_pts; //internal


    vec_double elev;
    vec_double width;
    vec_double q;
    vec_double h;


    vec_idx  next_rivers;
    vec_idx  prev_rivers;
    vec_bool reverse_next_rivers;
    vec_bool reverse_prev_rivers;

    ofVec2f pos_ini;
    ofVec2f pos_fin;
};
//]


//[ River_section
class river_section_t
{
public:
    double h;
    double b;

    double S(double z);
    double B(double z);
};
//]




// GRAPH_CLASS
class graph_t
{
    public:

//[graph variables
      idx_int n_rivers, n_nodes, n_sections;
      idx_int n_single_nodes, n_triple_nodes;
      idx_int n_discret_pts, n_int_variables, n_variables;
      double max_h, min_h, prop;
      vector <river_t> rivers;
      vector <node_t>  nodes;
      vector <river_section_t> sections;
      vec_idx border_nodes;

//]


//[functions to import/export data
        void load_structure(param_t *param);
        void load_elevation_and_width(param_t *param);

        void set_hq_constant(double h);
        void load_hq(string hq_file);
        void save_hq(string hq_file);


        void compute_minmax_h();
//
        ofVec2f graph_pos_particle(idx_int river, double pos_in_river, double lateral);

        ofVec2f graph_pos(idx_int river, double pos_in_river);

        double Q(idx_int river, double pos_in_river, bool normalized=true);//flow
        double H(idx_int river, double pos_in_river, bool normalized=true);//height
        double U(idx_int river, double pos_in_river, bool normalized=true);//speed
        double E(idx_int river, double pos_in_river, bool normalized=true);//elevation
};
//]








