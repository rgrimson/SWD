#include "Visualization.h"
#include "Graph.h"


//------------------------------------------------------------------
visualization_t::visualization_t(){

    mod1 = false;
    mod2 = false;
    mod3 = false;
    mod4 = false;


    n_interp_pts=5;
    pdx = 1/(float)(n_interp_pts-1);

}

void visualization_t::setup(graph_t *graph_in, param_t *param_in){

    graph = graph_in;
    param = param_in;

    /// River visual H

    river_shape_t aux;
    river_shape.assign(graph->n_rivers, aux);

    float px;
    ofVec2f v;
    ofVec2f p;


    for (idx_int r=0; r < graph->n_rivers;r++)//plot rivers heights
    {
        for (idx_int i=0; i < n_interp_pts; i++)
        {
            px = i*pdx;
            p = graph->graph_pos(r, px);

            if (i==n_interp_pts-1)
                v = p - graph->graph_pos(r, px-pdx);
            else
                v = graph->graph_pos(r, px+pdx) - p;

            river_shape[r].addMeshVertex(p,v,0.5);
        }
    }

    /// Particle visual

    particle.assign(param->max_particles, particle_t());
    resetParticles();
    //distribute initial particles
    for (idx_int r = 0; r<graph->n_rivers; r++)
    {
        idx_int ni=graph->rivers[r].n_int_discret_pts+1; //number of intervals
        double lr=graph->rivers[r].length;
        double li=lr/(double) ni;//length of each inteval
        for (idx_int i=0; i<ni; i++)
        {
            double w=li*(graph->rivers[r].h[i]+graph->rivers[r].h[i+1])/2;//falta tener en cuenta el ancho
            double k=w/param->inv_particles_density-0.5; //number of particles between these two discret pts
            double d=li/k; //distance between particles
            for (idx_int j=0; j<k; j++)
            {
                addParticle(r,(d*j+d/2+li*i)/lr,100);
            }
        }
    }
    //set accumulated flow in at boundary nodes to 0
    boundary_q_in.assign(graph->n_single_nodes,0.0);

}


//------------------------------------------------------------------
void visualization_t::reset(){

}

//------------------------------------------------------------------
void visualization_t::update(){//returs 1 if went out of the system

    float fc;
    int c;

    float px;

    if(mod1){

        for (idx_int r=0; r<graph->n_rivers;r++)//plot rivers heights
            {
                c = 0;
                for (idx_int i=0; i < n_interp_pts; i++)
                {
                    px = i*pdx;
                    fc = graph->H(r,px,true);
                    fc = ofMap(fc,0.0,1.0,0.0,255.0,true);

                    color = ofColor::fromHsb( 120, 255, fc );
                    color.a = 128;

                    river_shape[r].objectMesh.setColor(c,color);
                    c++;
                    river_shape[r].objectMesh.setColor(c,color);
                    c++;
                }
                river_shape[r].set_mesh();
            }
    }

    if(mod3){
        /** COMPUTE PARTICLES **/

        //if (param->verbose) cout << "Adding new input particles (" << inactive_particles.size() << " remaining)" << endl;
        for (idx_int i=0; i<graph->n_single_nodes; i++) // add particles if inflow
        {
            bool start=false;
            idx_int r;
            idx_int n = graph->border_nodes[i];  //let n be the i-th single node.
            if (graph->nodes[n].river_out.size()>0)  //if a river starts at n
            {
                start=true;
                r = graph->nodes[graph->border_nodes[i]].river_out[0]; //call it r
                double q=graph->rivers[r].q[0];
                if (q>0) //if the flow goes in,
                    boundary_q_in[i]+=q;  //increment accumulated flow.
            }
            else //else if a river ends here
            {
                r = graph->nodes[graph->border_nodes[i]].river_in[0]; //call it r
                double q=graph->rivers[r].q[graph->rivers[r].n_int_discret_pts+1];
                if (q<0) //if the flow goes in, add a particle there
                    boundary_q_in[i]-=q;  //increment accumulated flow.
            }

            while ((boundary_q_in[i])>param->inv_particles_density)
            {
                if (start)
                    addParticle(r,0,i);
                else
                    addParticle(r,1,i);
                boundary_q_in[i]-=param->inv_particles_density;
            }
        }

        //if (param->verbose) cout << "Moving and killing particles" << endl;
        for(idx_int i = 0; i < particle.size(); i++)
        {
            if (particle[i].active && particle[i].update(graph, param))
                killParticle(i);
        }
    }







/*
	graph->Q(river,pos_in_river,true);

	graph->graph_pos(river, pos_in_river, lateral, rand, &pos);

	intensity=graph->H(river,pos_in_river,true);

*/

}

//------------------------------------------------------------------
void visualization_t::draw(){

   // ofScale(50.0,1.0,1.0);

    if(mod1)
        for (idx_int r=0; r<graph->n_rivers;r++)
            river_shape[r].draw();

    if(mod3)
    {
        //ofScale(50.0,50.0,0.0);
        for(idx_int i = 0; i < particle.size(); i++){
            if (particle[i].active) particle[i].draw();
        }
    }
    /*
    if (param->draw_particles)
    {
        if (param->verbose) cout << "Drawing particles" << endl;




        ofSetColor(230);
        ofDrawBitmapString("Spacebar to reset. \n+ to add new points. \n\nEsc to exit.", 10, 20);
    }
    if (param->draw_locations)
    {
        double *qh;
        qh = new double[2];
        for (idx_int l=0; l<param->n_locations; l++)
        {
            solver.qh(locations.river[l],locations.pos_in_river[l],qh);
            ofCircle(l*100+100,50*qh[0]+300,5);
            ofCircle(l*100+150,50*qh[1]+300,5);
        }
    }
    if (param->draw_h_as_borders)
        for (idx_int r=0; r<graph->n_rivers;r++)//plot rivers heights
        {
            ofVec2f v(graph->rivers[r].pos_fin - graph->rivers[r].pos_ini);
            ofPolyline line;
            ofVec2f w=v;
            w.normalize();
            w.rotate(90);//,ofVec3f(0, 0, 1));
            w=w*5;

            //line.addVertex(graph->rivers[r].pos_fin);
            //line.addVertex(graph->rivers[r].pos_ini);
            for (idx_int i=0; i<graph->rivers[r].n_discret_pts; i++)
                line.addVertex(graph->rivers[r].pos_ini+v*i/(graph->rivers[r].n_discret_pts-1)-w*graph->rivers[r].h[i]);
            //line.close(); // close the shape
            line.draw();
            line.clear();
            for (idx_int i=0; i<graph->rivers[r].n_discret_pts; i++)
                line.addVertex(graph->rivers[r].pos_ini+v*i/(graph->rivers[r].n_discret_pts-1)+w*graph->rivers[r].h[i]);
            line.draw();
        }

*/
}


//--------------------------------------------------------------
void visualization_t::resetParticles(){
    inactive_particles.clear();
    for (idx_int i=0;i<param->max_particles;i++)
        inactive_particles.push_back(i);

    for(idx_int i = 0; i < particle.size(); i++)
        particle[i].reset();
}

//--------------------------------------------------------------
void visualization_t::addParticle(){
    int i;
    if (!inactive_particles.empty())
    {
        i=inactive_particles.back();
        inactive_particles.pop_back();
        particle[i].reset();
        particle[i].active=true;
    }
}

void visualization_t::addParticle(idx_int r, double pos, idx_int color){
    int i;
    if (!inactive_particles.empty())
    {
        i=inactive_particles.back();
        inactive_particles.pop_back();
        particle[i].reset();
        particle[i].river = r;
        particle[i].pos_in_river = pos;
        particle[i].active=true;
        particle[i].color=color;
    }
}

//--------------------------------------------------------------
void visualization_t::killParticle(int i){
    inactive_particles.push_back(i);
    particle[i].active=false;
}



