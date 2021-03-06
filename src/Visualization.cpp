#include "Visualization.h"
#include "Graph.h"


//------------------------------------------------------------------
visualization_t::visualization_t(){

    mod1 = false; //rivers
    mod2 = false;
    mod3 = true;  //particles
    mod4 = false;
    mod5 = false;
    mod6 = false;
    mod7 = false;
    mod8 = false;
    mod9 = false;
    mod0 = false;

    n_interp_pts=5;
    pdx = 1/(float)(n_interp_pts-1);

}

void visualization_t::setup(graph_t *graph_in, param_t *param_in, locations_t *locations_in){

    graph = graph_in;
    param = param_in;
    locations=locations_in;

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
            //revisar el ancho!
            river_shape[r].addMeshVertex(p,v,graph->sections[graph->rivers[r].section].B(graph->H(r, ((float)(i))/ n_interp_pts,false)*9/graph->prop));//el *7 no tiene nada que hacer aca
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
            double hi=graph->rivers[r].h[i];//heights
            double hf=graph->rivers[r].h[i+1];
            double si=graph->sections[graph->rivers[r].section].S(hi);//surfaces
            double sf=graph->sections[graph->rivers[r].section].S(hf);
            double w=li*(si+sf)/2;//volume
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

    if(mod3==mod3){ //cambie esto porque el update debe realizarse siempre, se dibuje o no
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
                    boundary_q_in[i]+=q*param->dt;  //increment accumulated flow.
            }
            else //else if a river ends here
            {
                r = graph->nodes[graph->border_nodes[i]].river_in[0]; //call it r
                double q=graph->rivers[r].q[graph->rivers[r].n_int_discret_pts+1];
                if (q<0) //if the flow goes in, add a particle there
                    boundary_q_in[i]-=q*param->dt;  //increment accumulated flow.
            }

            while ((boundary_q_in[i])>param->inv_particles_density)
            {
                if (start)
                    addParticle(r,ofRandom(0,graph->U(r,0)),i);
                else
                    addParticle(r,ofRandom(1-graph->U(r,1),1),i);
                boundary_q_in[i]-=param->inv_particles_density;
            }
        }

        //if (param->verbose) cout << "Moving and killing particles" << endl;
        for(idx_int i = 0; i < active_particles.size(); i++)
        {
            if (particle[active_particles[i]].update(graph, param))
                killParticle(active_particles[i]);
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

    if(mod8)
    {
        ofVec2f pos;
        ofSetColor(255, 0, 255, 127);

        for (idx_int r=0; r<graph->n_rivers;r++)
        {
            pos=graph->graph_pos(r,0);
            ofCircle(pos.x,pos.y,0.05);
            ofSetColor(0, 255, 255, 127);
            for (idx_int j=1; j<(graph->rivers[r].n_discret_pts)-1; j++)
            {
                pos=graph->graph_pos(r,((double) j)/(graph->rivers[r].n_discret_pts-1));
                ofCircle(pos.x,pos.y,0.05);
            }
            ofSetColor(255, 0, 255, 127);
            pos=graph->graph_pos(r,1);
            ofCircle(pos.x,pos.y,0.05);

        }
        ofSetColor(255, 255, 0, 127);
        for (idx_int i=0; i<graph->n_single_nodes;i++)
        {
            idx_int n=graph->border_nodes[i];
            idx_int r;
            double pos_in_river=0;

            if (graph->nodes[n].river_out.empty())
            {
                r=graph->nodes[n].river_in[0];
                pos_in_river=1;
            }
            else
                r=graph->nodes[n].river_out[0];

            pos=graph->graph_pos(r,pos_in_river);
            ofCircle(pos.x,pos.y,0.05);
        }

    }

    if(mod7){
        ofSetColor(0, 255, 0, 127);
        for (idx_int i=0;i<locations->n_locations;i++)
        {
            ofCircle(locations->pos[i].x, locations->pos[i].y, 0.1);
        }
            //locations->draw();
    }

    if(mod3)
    {
        //ofScale(50.0,50.0,0.0);
        for(idx_int i = 0; i < active_particles.size(); i++)
            particle[active_particles[i]].draw();
    }


}


//--------------------------------------------------------------
void visualization_t::resetParticles(){
    inactive_particles.clear();
    active_particles.clear();
    idx_particles.clear();
    for (idx_int i=0;i<param->max_particles;i++)
    {
        inactive_particles.push_back(i);
        idx_particles.push_back(0);
    }


    for(idx_int i = 0; i < particle.size(); i++)
        particle[i].reset();
}

//--------------------------------------------------------------
void visualization_t::addParticle(idx_int r, double pos, idx_int color){
    int i;
    if (!inactive_particles.empty())
    {
        i=inactive_particles.back();
        inactive_particles.pop_back();
        active_particles.push_back(i);
        idx_particles[i]=active_particles.size()-1;
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
    idx_int j=idx_particles[i];//pos in vector
    swap(active_particles[j], active_particles.back());
    active_particles.pop_back();
    idx_particles[active_particles[j]]=j;
}



