#pragma once

#include "Solver2.h"
#include "ofMain.h"
#include "Particle.h"
#include "Graph.h"
#include "Boundary.h"
#include "Locations.h"
#include "Visualization.h"


class testApp : public ofBaseApp  {

        idx_int t;
        param_t param;
        graph_t graph;
        solver_t solver;
        boundaries_t boundaries;
        locations_t locations;
        visualization_t visualization;
        string model_data_file;
        string model_data_dir;

        float zscale;
        float ztarget;


	public:
		void setup();
		void load_bc();
		void update();
		void draw();
		void verbose();

        void setModelDataFile(string model_dir, string model_data_in);
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

};
