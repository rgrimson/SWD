#include <fstream>
#include <iostream>
#include "stdio.h"
#include "ofMain.h"
#include "ofAppGlutWindow.h"

#include "Graph.h"
#include "Model.h"
//========================================================================


//========================================================================
int main(int argc, char *argv[]){
    //check arguments
    bool verbose=false;
    if (argc > 1)
		{
			idx_int len = strlen(argv[1]);
			string aux = string(argv[1]);

			for(idx_int i = 0; i < len;i++)
			{
				if( aux.compare(i,1,"v") == 0)
				{
					verbose = true;
					cout << "Verbose" << endl;
				}
			}
		}

    //create the model's appilication
    testApp *Model;
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context
	Model=new testApp();
	if (verbose) Model->verbose();
	//Model->setModelDataFile("../models/simple_delta/","ModelData.in");
	//Model->setModelDataFile("../models/2parallels/","ModelData.in");
  	Model->setModelDataFile("../models/simple_graph/","ModelData.in");




	ofRunApp(Model);
}
