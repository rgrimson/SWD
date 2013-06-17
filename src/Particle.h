#pragma once
#include "ofMain.h"
#include "Graph.h"
#include "Parameters.h"


class particle_t{


	public:
		particle_t();

		void reset();
		int update(graph_t *graph, param_t *param);
		void draw();

        bool active;
        idx_int river;
        double pos_in_river; // belongs to [0,1)
        double lateral;
        double rand;
        idx_int color;
        double intensity;

		ofVec2f pos;
		float radius;
		idx_int particle_alpha;

};
