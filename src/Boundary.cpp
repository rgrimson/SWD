#include "Boundary.h"

//READS BOUNDARY CONDITIONS
void boundaries_t::load(param_t *param)
{
    string aux_str;
    ifstream file;
    n_boundaries=param->n_single_nodes;
    //Allocate space and set to 0 the boundary conditions
    cboundary = new double[n_boundaries*param->nt];
    for (idx_int i = 0; i<n_boundaries*param->nt; i++)
        cboundary[i]=0;

    if (param->verbose) cout << "Reading boundary conditions" << endl;
    //read borders.in
    file.open((param->model_dir+param->borders_dir+param->borders_file).c_str());
    for(idx_int i = 0; i < n_boundaries; i++)
    {
        idx_int cyclic;
        idx_int k;
        file >> aux_str >> k >> cyclic;
        if (param->verbose) cout << "reading " << k << " values for boundary " << aux_str << " (cyclic = " << cyclic << ")." <<endl;
        for (idx_int j=0; (j<k && j<param->nt); j++)  //read k conditions
            file >> cboundary[j*param->n_single_nodes+i];
        if (cyclic) //set the remaining conditions if we have to
            for (idx_int j=k;j<param->nt;j++)
                cboundary[j*param->n_single_nodes+i]=cboundary[(j%k)*param->n_single_nodes+i];


    }
    file.close();
}

double *boundaries_t::at_time(idx_int t)
{
  return cboundary+t*n_boundaries;
}
