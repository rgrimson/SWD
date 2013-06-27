#include "Particle.h"
#include "Graph.h"


//------------------------------------------------------------------
particle_t::particle_t(){

    particle_alpha = 128;

}

//------------------------------------------------------------------
void particle_t::reset(){
	//the unique val allows us to set properties slightly differently for each particle_t

	//pos.x = ofRandomWidth();
	//pos.y = ofRandomHeight();

	//vel.x = ofRandom(-3.9, 5.0);
	//vel.y = ofRandom(-3.9, 3.9);

	radius = 0.02;
	river = 0;
	pos_in_river = 0;
	lateral = ofRandom(-1,1);
	intensity=1;
	rand = ofRandom(10);
	active=false;
	color=255;
}

//------------------------------------------------------------------
int particle_t::update(graph_t *graph, param_t *param){//returs 1 if went out of the system

	//1 - APPLY THE FORCES BASED ON WHICH MODE WE ARE IN
	//double q=param->dt*graph->Q(river,pos_in_river,true);
	//double h=param->dt*graph->H(river,pos_in_river,true);
	//double s=graph->sections[river].S(h);
	//double sh=graph->sections[river].h;
	//double sb=graph->sections[river].b;
	double u=graph->U(river,pos_in_river,true);
	double dt=param->dt;
	pos_in_river+=dt*u;

	//cout <<"q:" << q <<" u:" << u << " h:" << h << " s:" << s <<" sh:" << sh <<" sb:" << sb << endl;

	// If the particle_t reacher a union point
    if (pos_in_river>=1)
    {
        double et=(pos_in_river-1)/u;//extra_time

        if ((graph->rivers[river].next_rivers).empty())  //no next river
         return 1;
        else if ((graph->rivers[river].next_rivers).size()==2)  //2 next river
        {
            //aca habria que calcular cuanto se mete exatamente en el nuevo rio
            //double d1;
            //  d1=(pos_in_river-1)*length(river)/U(step,river,1]
            double q1,q2;
            bool r1=true,r2=true;

            if (graph->rivers[river].reverse_next_rivers[0])// compute the flow of the adjacent rivers
                q1=-graph->Q(graph->rivers[river].next_rivers[0],1);
            else
                q1=graph->Q(graph->rivers[river].next_rivers[0],0);

            if (graph->rivers[river].reverse_next_rivers[1])
                q2=-graph->Q(graph->rivers[river].next_rivers[1],1);
            else
                q2=graph->Q(graph->rivers[river].next_rivers[1],0);

            if (q1<0)
                r1=false;
            if (q2<0)
                r2=false;

            if (r1 && r2)
            {
                if (ofRandom(-q1,q2)<0)
                    r2=false;
                else
                    r1=false;
            }

            idx_int new_river=river;
            bool reverse_river=false;

            if (r1 && !r2)  //takes the first river
            {
              new_river=graph->rivers[river].next_rivers[0];
              if (graph->rivers[river].reverse_next_rivers[0])
                reverse_river=true;
            }

            else if (r2 && !r1)  //takes the second river
            {
              new_river=graph->rivers[river].next_rivers[1];
              if (graph->rivers[river].reverse_next_rivers[1])
                reverse_river=true;
            }
            else if (!r1 && !r2)  //stays in the union
            {
              pos_in_river=1;  //this should not happen
              et=0;
            }

            river=new_river;
            if (reverse_river)
                pos_in_river=1 + et * graph->U(new_river,1,true);
            else
                pos_in_river = et * graph->U(new_river,0,true);

        }
    }

// If the particle_t reacher a union point
    if (pos_in_river<0)
    {
        double et=-pos_in_river/u;//extra_time

        if ((graph->rivers[river].prev_rivers).empty())  //no prev river
         return 1;
        else if ((graph->rivers[river].prev_rivers).size()==2)  //2 prev rivers
        {
            //aca habria que calcular cuanto se mete exatamente en el nuevo rio
            //double d1;
            //  d1=(pos_in_river-1)*length(river)/U(step,river,1]
            double q1,q2;
            bool r1=true,r2=true;

            if (graph->rivers[river].reverse_prev_rivers[0])// compute the flow of the adjacent rivers
                q1=graph->Q(graph->rivers[river].prev_rivers[0],0);
            else
                q1=-graph->Q(graph->rivers[river].prev_rivers[0],1);

            if (graph->rivers[river].reverse_prev_rivers[1])
                q2=graph->Q(graph->rivers[river].prev_rivers[1],0);
            else
                q2=-graph->Q(graph->rivers[river].prev_rivers[1],1);

            if (q1<0)
                r1=false;
            if (q2<0)
                r2=false;

            if (r1 && r2)
            {
                if (ofRandom(-q1,q2)<0)
                    r2=false;
                else
                    r1=false;
            }

            idx_int new_river=river;
            bool reverse_river=false;

            if (r1 && !r2)  //takes the first river
            {
              if (graph->rivers[river].reverse_prev_rivers[0])
                reverse_river=true;
              new_river=graph->rivers[river].prev_rivers[0];
            }

            else if (r2 && !r1)  //takes the second river
            {
              if (graph->rivers[river].reverse_prev_rivers[1])
                reverse_river=true;
              new_river=graph->rivers[river].prev_rivers[1];
            }
            else if (!r1 && !r2)  //stays in the union
            {
              pos_in_river=1;  //this should not happen
              et=0;
            }

            river=new_river;
            if (reverse_river)
                pos_in_river= - et * graph->U(new_river,0,true);
            else
                pos_in_river = 1 - et * graph->U(new_river,1,true);


        }
    }

    //2 - UPDATE OUR POSITION
	pos = graph->graph_pos_particle(river, pos_in_river, lateral);

	intensity=graph->H(river,pos_in_river,true);


	//3 - (optional) LIMIT THE PARTICLES TO STAY ON SCREEN
	//we could also pass in bounds to check - or alternatively do this at the testApp level
	/*if( pos.x > ofGetWidth() ){
		pos.x = 0;
	}else if( pos.x < 0 ){
		pos.x = ofGetWidth();
	}
	if( pos.y > ofGetHeight() ){
		pos.y = ofGetHeight();
	}
	else if( pos.y < 0 ){
		pos.y = 0;
	}*/

  return 0;

}

//------------------------------------------------------------------
void particle_t::draw(){
    idx_int m=(idx_int)(255*intensity);
    switch( color )
        {
            case 0:
                ofSetColor(m, 0, 0, particle_alpha);
                break;
            case 1:
                ofSetColor(0, m, 0, particle_alpha);
                break;
            case 2:
                ofSetColor(0, 0, m, particle_alpha);
                break;
            case 3:
                ofSetColor(m, m, 0, particle_alpha);
                break;
            case 4:
                ofSetColor(m, 0, m, particle_alpha);
                break;
            case 5:
                ofSetColor(0, m, m, particle_alpha);
                break;
            case 6:
                ofSetColor(m, m, m, particle_alpha);
                break;
            default :
                ofSetColor(m, m, m, particle_alpha);
}
	ofCircle(pos.x, pos.y, radius);
}

