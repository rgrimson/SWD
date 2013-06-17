#pragma once

#include <vector>
#include "ofMain.h"
using namespace std;

// type for idxex of rivers or nodes or small? things
typedef unsigned int idx_int;

/********************/
/*** PARAMETERS *****/
/********************/
class param_t
{
    //MACROS #define N_RIVERS(i) (rivers[(i)].n_int_discret_pts)
    public:

      /** VARIABLES **/
      /** Internal parameters **/
      double g;
      bool verbose;

      /** draw parameters **/
      idx_int sleep_time_per_cicle;
      bool compute_hq;
      bool compute_particles;
      bool draw_particles;
      bool draw_h_as_borders;
      bool draw_locations;
      bool draw_other;
      bool draw;
      bool save_locations;
      bool save_hq;
      double plot_scale;
      double inv_particles_density;
      idx_int max_particles;


      /** model parameters from file**/
      double dt,cm,theta;
      idx_int n_rivers, n_sections, n_nodes, n_single_nodes, n_locations;
      idx_int nt;
      string model_dir;
      string graph_dir;
      string rivers_file;
      string sections_file;
      string nodes_file;
      string locations_file;
      string elevation_file;
      string width_file;

      string borders_dir;
      string borders_file;

      string out_dir;
      string locations_out_file;



      /** LOAD **/
      void load_parameters(string datadir, string parameters_file);
      void set_internal_parameters();
};
//]
