#include "Graph.h"
#include <fstream>

//========================================================================
void  param_t::set_internal_parameters()
{
    save_locations    = true;
    save_hq           = false;
    draw              = true;
    compute_particles = draw_particles;

    compute_hq        =  save_hq || draw;

    sleep_time_per_cicle=0;
    plot_scale=100;
    g=9.8;

    //particles parameters
    max_particles=100000;

    //when a border flows in, add points at this rate
    inv_particles_density = 500;//m^3 of water/particle
}

//========================================================================
void  param_t::load_parameters(string modeldir,string parameters_file)
{
    string aux_str;
    ifstream file;
    file.open((modeldir + parameters_file).c_str());
    model_dir=modeldir;

    file >> aux_str >> aux_str >> n_rivers;
    if (verbose) cout << "n_rivers = " << n_rivers << endl;
    file >> aux_str >> aux_str >> n_sections;
    if (verbose) cout << "n_river_sections = " << n_sections << endl;
    file >> aux_str >> aux_str >> n_nodes;
    if (verbose) cout << "n_nodes = " << n_nodes << endl;
    file >> aux_str >> aux_str >> n_single_nodes;
    if (verbose) cout << "n_boundary = " << n_single_nodes << endl;
    file >> aux_str >> aux_str >> n_locations;
    if (verbose) cout << "n_locations = " << n_locations << endl;
    file >> aux_str >> aux_str >> theta;
    if (verbose) cout << "theta = " << theta << endl;
    file >> aux_str >> aux_str >> cm;
    if (verbose) cout << "cm = " << cm << endl;
    file >> aux_str >> aux_str >> dt;
    if (verbose) cout << "dt = " << dt << endl;
    file >> aux_str >> aux_str >> nt;
    if (verbose) cout << "nt = " << nt << endl;
    file >> aux_str >> aux_str >> graph_dir;
    if (verbose) cout << "graph_dir = " << graph_dir << endl;
    file >> aux_str >> aux_str >> rivers_file;
    if (verbose) cout << "rivers_file = " << rivers_file << endl;
    file >> aux_str >> aux_str >> sections_file;
    if (verbose) cout << "sections_file = " << sections_file << endl;
    file >> aux_str >> aux_str >> nodes_file;
    if (verbose) cout << "nodes_file = " << nodes_file << endl;
    file >> aux_str >> aux_str >> locations_file;
    if (verbose) cout << "locations_file = " << locations_file << endl;
    file >> aux_str >> aux_str >> borders_dir;
    if (verbose) cout << "borders_dir = " << borders_dir << endl;
    file >> aux_str >> aux_str >> borders_file;
    if (verbose) cout << "borders_file = " << borders_file << endl;
    file >> aux_str >> aux_str >> out_dir;
    if (verbose) cout << "output_dir = " << out_dir << endl;
    file >> aux_str >> aux_str >> locations_out_file;
    if (verbose) cout << "locations_out_file = " << locations_out_file << endl;


    file.close();
}
