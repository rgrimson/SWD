#pragma once
#include "ofMain.h"
#include "Graph.h"
#include "Parameters.h"
#include "Particle.h"
#include "River_shape.h"

class visualization_t{


	public:
		visualization_t();

		void reset();
		void setup(graph_t *graph, param_t *param);
		void update();
		void draw();

        void resetParticles();
		void addParticle();
		void addParticle(idx_int r, double pos, idx_int color);
		void killParticle(int i);


		bool mod1;
		bool mod2;
		bool mod3;
		bool mod4;

		float pdx;

		graph_t *graph;
		param_t *param;

		idx_int n_interp_pts;

		ofColor color;

		vector <river_shape_t> river_shape;

		        //to manage particles
		vector <particle_t> particle;
		vector <int> inactive_particles;
        vector <double> boundary_q_in;

};
