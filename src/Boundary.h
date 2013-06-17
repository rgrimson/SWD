#pragma once

//#include <vector>
//#include "ofMain.h"
#include "Parameters.h"

using namespace std;

/********************/
/*** Boundaries *****/
/********************/
class boundaries_t
{
    idx_int n_boundaries;
    /** VARIABLES **/
    double *cboundary;

public:
      /** LOAD **/
      void load(param_t *param);

      double *at_time(idx_int t);
};
//]
