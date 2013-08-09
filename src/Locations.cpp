#include "Locations.h"

//===============================================================
//===============================================================
void locations_t::load(param_t *param_in, solver_t *solver_in, graph_t * graph)
{
    string aux_str;
    ifstream file;
    solver=solver_in;
    param=param_in;
    n_locations=param->n_locations;


    //Allocate space and set to 0 the boundary conditions
    name = new string[n_locations];
    river = new idx_int[n_locations];
    pos_in_river = new double[n_locations];
    pos = new ofVec2f[n_locations];

    if (param->verbose) cout << "Reading locations of output stations from " << (param->model_dir+param->graph_dir+param->locations_file) << endl;
    //read borders.in
    file.open((param->model_dir+param->graph_dir+param->locations_file).c_str());

    for(idx_int i = 0; i < param->n_locations; i++)
    {
        file >> aux_str;
        file >> aux_str >>  name[i];
        file >> aux_str;
        file >> aux_str >>  river[i];
        file >> aux_str;
        file >> aux_str >>  pos_in_river[i];
        if (param->verbose) cout << "Location " << i << ": " << name[i] << ", " << river[i] << ", " << pos_in_river[i] << endl;
        pos[i] = graph->graph_pos_particle(river[i], pos_in_river[i], 0);
    }
    file.close();

}

//===============================================================
//===============================================================
void locations_t::open_output_file()
{
        out_file = fopen ((param->model_dir+param->out_dir+param->locations_out_file).c_str(), "wb");
}

//===============================================================
//===============================================================

void locations_t::save_step()
{
    double *buffer;
    buffer = new double[2];
    for (idx_int i=0;i<n_locations;i++)
    {
        solver->qh(river[i],pos_in_river[i],buffer);
        fwrite (buffer , sizeof(double), 2, out_file);
    }

}
//===============================================================
//===============================================================

locations_t::~locations_t()
{
    fclose(out_file);
}


