#pragma once
#include "ofMain.h"
#include "Graph.h"
#include "Parameters.h"
#include "Particle.h"
#include "River_shape.h"
#include "Locations.h"

class visualization_t{


	public:
		visualization_t();

		void reset();
		void setup(graph_t *graph, param_t *param, locations_t *locations);
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
		bool mod5;
		bool mod6;
		bool mod7;
		bool mod8;
		bool mod9;
		bool mod0;

		float pdx;

		graph_t *graph;
		param_t *param;
		locations_t *locations;

		idx_int n_interp_pts;

		ofColor color;

		vector <river_shape_t> river_shape;

		        //to manage particles
		vector <particle_t> particle;
		vector <int> inactive_particles;
		vector <int> active_particles;
        vector <int> idx_particles;
        vector <double> boundary_q_in;


};
