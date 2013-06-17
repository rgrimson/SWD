#include "Graph.h"
#include <fstream>

void  graph_t::load_structure(param_t *param)
{
    string aux_str;
    ifstream file;
    //dimension vectors
    n_nodes=param->n_nodes;
    n_rivers=param->n_rivers;
    n_sections=param->n_sections;


    rivers.resize(n_rivers);
    sections.resize(n_sections);
    nodes.resize(n_nodes);

    ofVec2f aux_pt;
    //read nodes.in
    file.open((param->model_dir + param->graph_dir + param->nodes_file).c_str());
    for(idx_int i = 0; i < n_nodes; i++)
    {

        file >>  nodes[i].pos.y >> nodes[i].pos.x >> nodes[i].degree;

    }

    file.close();

    ofVec2f pt(0,0);

    //read sections.in
    file.open((param->model_dir + param->graph_dir + param->sections_file).c_str());
    for(idx_int i = 0; i < n_sections; i++)
        file >> aux_str >> sections[i].h >> sections[i].b;
    file.close();


    //read rivers.in
    file.open((param->model_dir + param->graph_dir + param->rivers_file).c_str());
    file >> aux_str >> aux_str >> aux_str >> aux_str >> aux_str>> aux_str;
    for(idx_int i = 0; i < n_rivers; i++)
        file >> rivers[i].name >> rivers[i].node_start >> rivers[i].node_end >> rivers[i].length >>  rivers[i].section >> rivers[i].n_int_discret_pts;
    file.close();

    for (idx_int i=0; i<(n_rivers); i++)  //define dx, plot positions etc for each river.
    {
        rivers[i].dx=rivers[i].length/((double)(rivers[i].n_int_discret_pts+1));
        rivers[i].n_discret_pts=rivers[i].n_int_discret_pts+2;

        pt.x=nodes[rivers[i].node_start].pos.x;
        pt.y=nodes[rivers[i].node_start].pos.y;
        rivers[i].pos_ini=pt;

        pt.x=nodes[rivers[i].node_end].pos.x;
        pt.y=nodes[rivers[i].node_end].pos.y;
        rivers[i].pos_fin=pt;
    }
    for (idx_int i=0; i<(n_rivers-1); i++)  //compute the adjacency information from the rivers data
        for (idx_int j=i+1; j<n_rivers; j++)
        {
            if (rivers[i].node_start==rivers[j].node_start)
            {
                rivers[i].prev_rivers.push_back(j);
                rivers[i].reverse_prev_rivers.push_back(true);
                rivers[j].prev_rivers.push_back(i);
                rivers[j].reverse_prev_rivers.push_back(true);
            }
            if (rivers[i].node_start==rivers[j].node_end)
            {
                rivers[i].prev_rivers.push_back(j);
                rivers[i].reverse_prev_rivers.push_back(false);
                rivers[j].next_rivers.push_back(i);
                rivers[j].reverse_next_rivers.push_back(false);
                            }
            if (rivers[i].node_end==rivers[j].node_end)
            {
                rivers[i].next_rivers.push_back(j);
                rivers[i].reverse_next_rivers.push_back(true);
                rivers[j].next_rivers.push_back(i);
                rivers[j].reverse_next_rivers.push_back(true);
            }
            if (rivers[j].node_start==rivers[i].node_end)
            {
                rivers[j].prev_rivers.push_back(i);
                rivers[i].reverse_next_rivers.push_back(false);
                rivers[i].next_rivers.push_back(j);
                rivers[j].reverse_prev_rivers.push_back(false);
            }
    }


    for (idx_int r=0; r<(n_rivers); r++)  // define nodes information
    {
        nodes[rivers[r].node_start].river_out.push_back(r);
        nodes[rivers[r].node_end].river_in.push_back(r);
    }
    n_triple_nodes = 0;  //hacer el calculo correcto.
    n_single_nodes = 0;

    for (idx_int n=0; n<(n_nodes); n++)  //compute the number of borders
    {
        int d=nodes[n].river_in.size()+nodes[n].river_out.size(); //degree
        if (d==1)
        {
            n_single_nodes++;
            border_nodes.push_back(n);
        }
        else if (d==3)
           n_triple_nodes++;
    }
    if (n_single_nodes+n_triple_nodes!=n_nodes)
    {
        cout << "Error: not every node is single or triple" << endl;
    }

    param->n_single_nodes=n_single_nodes;

    //set minimum and maximum heights to something reasonable
    max_h=2.1;
    min_h=1.9;
}

ofVec2f graph_t::graph_pos(idx_int r, double pos_in_river)
{
    if (pos_in_river<0 || pos_in_river>1)
        cout <<"out of bounds "<< pos_in_river  << endl;

    ofVec2f v(pos_in_river*rivers[r].pos_fin + (1-pos_in_river) * rivers[r].pos_ini);
    return v;
}


ofVec2f  graph_t::graph_pos_particle(idx_int r, double pos_in_river, double lateral)
{
    if (pos_in_river<0 || pos_in_river>1)
        cout <<"out of bounds" << endl;
    ofVec2f v(pos_in_river*rivers[r].pos_fin + (1-pos_in_river) * rivers[r].pos_ini);
    ofVec2f p(v);

    v=rivers[r].pos_fin - rivers[r].pos_ini;
    v.rotate(90);
    v.normalize();
    p = p + v*lateral;

    return p;

}

double graph_t::Q(idx_int river, double pos_in_river, bool normalized)
{
  //return 0.005;
  if (pos_in_river==1)
  {
    if (normalized)
     return (rivers[river].q[rivers[river].n_discret_pts -1])/rivers[river].length;
    else
     return (rivers[river].q[rivers[river].n_discret_pts -1]);
  }

  if (pos_in_river==0)
  {
    if (normalized)
     return (rivers[river].q[0])/rivers[river].length;
    else
     return (rivers[river].q[0]);
  }

  double frac, node;
  frac = modf (pos_in_river * (rivers[river].n_discret_pts -1 ), &node);
  if (normalized)
     return ((1-frac)*rivers[river].q[node]+(frac)*rivers[river].q[node+1])/rivers[river].length;
  return (1-frac)*rivers[river].q[node]+(frac)*rivers[river].q[node+1];
}


double graph_t::H(idx_int river, double pos_in_river, bool normalized)
{

  //return 0.005;
  if (pos_in_river==1)
  {
    if (normalized)
     return (rivers[river].h[rivers[river].n_discret_pts -1]-min_h)/(max_h-min_h);
    else
     return (rivers[river].h[rivers[river].n_discret_pts -1]);
  }

  if (pos_in_river==0)
  {
    if (normalized)
     return (rivers[river].h[0]-min_h)/(max_h-min_h);
    else
     return (rivers[river].h[0]);
  }

  double frac;
  double node;
  frac = modf (pos_in_river * (rivers[river].n_discret_pts - 1), &node);
  if (normalized)
     return ((1-frac)*rivers[river].h[node]+(frac)*rivers[river].h[node+1]-min_h)/(max_h-min_h);
  return (1-frac)*rivers[river].h[node]+(frac)*rivers[river].h[node+1];
}

void graph_t::set_hq_constant(double h)
{
    //flow and heights
    for (idx_int i=0; i<(n_rivers); i++)
        for (idx_int j=0;j<rivers[i].n_discret_pts; j++)
        {
            (rivers[i].q).push_back(0);
            (rivers[i].h).push_back(h);
        }
}
void graph_t::load_hq(string hq_file)
{

}
void graph_t::save_hq(string hq_file)
{
}
//

void graph_t::load_elevation_and_width(param_t *param)
{
    double aux_double;
    ifstream file;
    file.open((param->model_dir + param->elevation_file).c_str());

        for(idx_int i = 0; i < n_rivers; i++)
		{

            for (idx_int j = 0; j < rivers[i].n_discret_pts;j++)
            {
                file >> aux_double;
                rivers[i].elev.push_back(aux_double);
            }
        }

        file.close();
}

void graph_t::compute_minmax_h()
{
    double h;
    for (idx_int r=0; r<n_rivers; r++)
    {
        idx_int lr=rivers[r].n_discret_pts;
        for (idx_int n=0; n<lr; n++)
        {
            h=rivers[r].h[n];
            if (h>max_h) max_h=h;
            if (h<min_h) min_h=h;
        }
    }
    //min_h=min_h-(max_h-min_h)/2.0;
    //cout <<min_h << " " <<max_h << endl;
    //if (min_h<0) min_h=0;
}
/*******************
*** River_sections ***
********************/
double river_section_t::S(double z)
{
    if (z>h)
        return (z-h/2)*b+h;
    return b*z*z/2/h;
}

double river_section_t::B(double z)
{
        if (z>h)
             return b;
        return z*b;
}
