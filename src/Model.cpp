#include "Model.h"

//--------------------------------------------------------------
void testApp::setup(){
    if (param.verbose) cout << "Model setup" << endl;
    ofSetVerticalSync(true);
    ofSetWindowTitle("SWD");
    ofSetFrameRate(100);
    ofEnableAlphaBlending();

    t=0;

    //read parameters and graph
    if (param.verbose) cout << "Using input directory " << model_data_dir << endl;
    if (param.verbose) cout << "Using input file " << model_data_file << endl;
    param.load_parameters(model_data_dir , model_data_file);
    param.set_internal_parameters();

    //load graph structure
    if (param.verbose) cout << "Loading graph structure" << endl;
    graph.load_structure(&param);//"../data/rivers.in","../data/nodes.in");
    graph.set_hq_constant(2.0);

    //load boundary conditions
    boundaries.load(&param);

    //setup solver
    solver.setup(&graph,&param);

    //load loactions
    locations.load(&param,&solver);
    locations.open_output_file();

    /** SAVE initial condition for locations**/
    if (param.save_locations)
    {
        if (param.verbose) cout << "Saving locations data" << endl;
        locations.save_step();
    }
 	
	if(param.draw)
    {
        visualization.setup(&graph,&param);
    }

    zscale = 50;
    ztarget = 50;

}

//--------------------------------------------------------------
void testApp::update(){

    /** SOLVE **/
    if (param.verbose) cout << "Solving time step " << t << " ... " ;

    solver.solve(boundaries.at_time(t));

    if (param.verbose) cout << "done" << endl;
//exitApp();


    if (param.compute_hq)
        solver.w2hq(&graph);


    if(param.draw)
    {
        visualization.update();
    }

    /** SAVE **/
    if (param.save_locations)
    {
        if (param.verbose) cout << "Saving locations data" << endl;
        locations.save_step();
    }

    if (param.save_hq)
        graph.save_hq("../data/hq.out");

    /** GENERAL **/
    ofSleepMillis(param.sleep_time_per_cicle);

    if (param.verbose) cout << "Computing min and max_h" << endl;
    graph.compute_minmax_h();

    t++;
    if (t==param.nt)
        exitApp();

}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));


    if(param.draw)
    {
        ofTranslate(ofGetWindowWidth()/2.0,ofGetWindowHeight()/2.0);
        ofScale(zscale,zscale,0.0);
        visualization.draw();
    }


}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if( key == ' ' ){
        visualization.resetParticles();
    }
    if( key == '+' ){
        visualization.addParticle();
    }
    if( key == '-' ){
        idx_int i;
        for (i=0;(i<visualization.particle.size() && !visualization.particle[i].active);i++);
        if (i<visualization.particle.size()) visualization.killParticle(i);
    }

    if( key == '1' )
        visualization.mod1 = !visualization.mod1;
    if( key == '2' )
        visualization.mod2 = !visualization.mod2;
    if( key == '3' )
        visualization.mod3 = !visualization.mod3;

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

if (button==3)
    ztarget = zscale*1.3;

if (button==4)
    ztarget = zscale/1.3;

}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}


void testApp::verbose()
{
    param.verbose=1;
}

void testApp::setModelDataFile(string model_dir, string model_data_in)
{
    model_data_dir  = model_dir;
    model_data_file = model_data_in;
}
