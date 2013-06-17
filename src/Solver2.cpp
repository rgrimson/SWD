#include "Solver2.h"

void solver_t::setup(graph_t *graph_in, param_t *param_in)
{
    param=param_in;
    graph=graph_in;
    define_w_index(graph);

    //definir las variables A, b y w,
    //mat A;
    A.zeros(dimx,dimx);
    //mat b;
    b.zeros(dimx,1);

    //A: relacion de caudal en los nodos (no dependen de t)
    for (idx_int i=0;i<graph->n_nodes;i++)
    {
        for (idx_int j = 0; j < graph->nodes[i].river_in.size(); j++)
             A(graph->n_int_variables+i,w_idx[ graph->nodes[i].river_in[j] ] + 2*graph->rivers[graph->nodes[i].river_in[j]].n_int_discret_pts + 1) = -1;

        for (idx_int j=0;j<graph->nodes[i].river_out.size();j++)
             A(graph->n_int_variables+i,w_idx[ graph->nodes[i].river_out[j] ] )=1;
    }

    if (1==0)
            {
                ofstream fout_A("../output/A.out",ios::out | ios::binary);
                A.save(fout_A,raw_ascii);
            }
    //*****************
    //*** W INICIAL ***
    //*****************

    //mat w;
    w.zeros(dimx,1);

    set_w_constant(2, graph);
    hq2w(graph);
    //define index


    //escribir en A las condiciones fijas (kirchoff)
    //escribir el w inicial a partir de hq
}


void solver_t::solve(double *cboundary)
{
    //define_A_b(&A,&b), solve
    //cout << "Solver 2" << endl;
    if (1==0)
            {
                ofstream fout_w("../data/w.out",ios::out | ios::binary);
                w.save(fout_w,raw_ascii);
            }


        //qi  q1 h1 ...  qn    hn    qf
        //qi(r) = w(t,rivers[r].w_idx);
        //q(r,j)= w(t,rivers[r].w_idx+2*j+1);
        //qf(r) = w(t,rivers[r].w_idx+2*rivers[r].n_discret_pts+1);
        //hi(r) = w(t,n_int_variables+rivers[r].node_start)
        //h(r,j)= w(t,rivers[r].w_idx+2*j+2);
        //hf(r) = w(t,n_int_variables+rivers[r].node_end)

    //Defino A
    idx_int pos;
    idx_int lr, hi_idx, qi_idx, hj_idx, qj_idx, hf_idx, qf_idx, hs_idx, qs_idx;

    double theta=param->theta;
    double g=param->g;
    double dm1_6 = pow(0.0001,1/6.0); //diametro en metros delas partículas del fondo a la 1/6;


    double bj, bs, sj, ss;
    double hj, hs, qj, qs;
    double dj, ds, ddj, dds;



    //DEFINO A PARA EL w DADO
    for (idx_int r = 0; r<graph->n_rivers;r++)
    {
        double dx=graph->rivers[r].dx;
        double l=2*param->dt;
        idx_int section_idx=graph->rivers[r].section;

        lr = graph->rivers[r].n_int_discret_pts;
        pos = w_idx[r];

        qi_idx = pos;
        hi_idx = n_int_variables+graph->rivers[r].node_start;
        qf_idx = pos+2*lr+1;
        hf_idx = n_int_variables+graph->rivers[r].node_end;


        for (idx_int j=0;j<=lr;j++)//ecuaciones del nodo j del i esimo rio
         {
            if (j==0)
            {
                hj_idx=hi_idx;
                qj_idx=qi_idx;
            }
            else
            {
                qj_idx = pos + 2*j - 1;
                hj_idx = pos + 2*j;
            }


            if (j==lr)
            {
                hs_idx=hf_idx;
                qs_idx=qf_idx;
            }
            else
            {
                qs_idx=pos+2*j+1;
                hs_idx=pos+2*j+2;
            }

            hj = w[hj_idx];
            hs = w[hs_idx];
            qj = w[qj_idx];
            qs = w[qs_idx];
            sj = graph->sections[section_idx].S(hj);  //calcular el area
            ss = graph->sections[section_idx].S(hs);
            bj = graph->sections[section_idx].B(hj);   //calcular el ancho;
            bs = graph->sections[section_idx].B(hs);;
            dj = 21*sj*pow(hj,2/3.0)/dm1_6;
            ds = 21*ss*pow(hs,2/3.0)/dm1_6;;
            ddj= 0;//poner la derivada!
            dds= 0;

            A(pos+2*j  , qj_idx) =  -l;
            A(pos+2*j  , hj_idx) =   dx * bj / theta;
            A(pos+2*j  , qs_idx) =   l;
            A(pos+2*j  , hs_idx) =   dx * bs / theta;
            b(pos+2*j          ) =   l  * (qj - qs) / theta;


            A(pos+2*j+1, qj_idx) =  dx / (g * sj) + l * theta * (qj / (g * sj * sj) + abs(qj) * dx / (dj * dj));
            A(pos+2*j+1, hj_idx) =  qj * bj * dx / (g * sj * sj) + l * theta * (bj * qj * qj / (g * sj * sj * sj) - 1 - ddj * dx * qj * abs(qj)/ (dj * dj * dj) );
            A(pos+2*j+1, qs_idx) =  dx / (g * ss) + l * theta * (qs / (g * ss * ss) + abs(qs) * dx / (ds * ds));
            A(pos+2*j+1, hs_idx) =  qs * bs * dx / (g * ss * ss) - l * theta * (bs * qs * qs / (g * ss * ss * ss) - 1 + dx * dds * qs * abs(qs)/ (ds * ds * ds));
            b(pos+2*j+1        ) =  l* ((qj*qj/(sj*sj) - qs*qs/(ss*ss))/(2*g) + hj - hs - dx/2 * (qj * abs(qj) / (dj * dj) + qs * abs(qs) / (ds * ds)));
         }
     }

    //condicion de borde en la altura de los nodos simples
    for(idx_int i = 0;i<graph->n_single_nodes; i++)
        b(n_int_variables+graph->border_nodes[i])=cboundary[i];

    //solve one step
    w = w + arma::solve(A,b);

    if (1==0)
    {
        ofstream fout_w("../models/w2.out",ios::out | ios::binary);
        w.save(fout_w,raw_ascii);
        ofstream fout_A("../models/A2.out",ios::out | ios::binary);
        A.save(fout_A,raw_ascii);
        ofstream fout_b("../models/b2.out",ios::out | ios::binary);
        b.save(fout_b,raw_ascii);
    }
}

void solver_t::define_w_index(graph_t *graph)
{
//REVISAR
    for (idx_int r=0;r<graph->n_rivers;r++)
    {
        n_int_variables+=2*(graph->rivers[r].n_int_discret_pts)+2;
    }
    dimx = n_int_variables + graph->n_nodes;

    w_idx.resize(graph->n_rivers);
    w_idx[0]=0;
    for (idx_int r=1;r<graph->n_rivers;r++)
    {
        w_idx[r]=w_idx[r-1]+2*(graph->rivers[r-1].n_int_discret_pts)+2;
    }
    graph->n_int_variables=n_int_variables;

}
//----------

void solver_t::set_w_constant(double h, graph_t *graph)
{
//pongo las alturas y caudales en 1
        idx_int pos = 0; //pos de la primera altura en w
        for (idx_int r = 0; r< (graph->n_rivers); r++) //de los nodos internos
        {
            w(pos)=0; //qi
            pos++;
            for (idx_int j = 0; j<(graph->rivers[r].n_int_discret_pts); j++) ////nodo j del i esimo rio
            {
                w(pos) = 0;   //caudal
                pos++;
                w(pos) = h;   //altura=h
                pos++;

            }
            w(pos)=0; //qf
            pos++;
        }

        for (idx_int n=0; n<(graph->n_nodes);n++) //de los nodos de confluencia y bordes
        {
            pos=n_int_variables+n;
            w(pos)=h;   //altura del i-esimo nodo de confluencia
        }
}
void solver_t::load_w()
{

}
void solver_t::hq2w(graph_t *graph)
{

}
void solver_t::w2hq(graph_t *graph)
{
    idx_int pos;
    idx_int lr, hi_idx, qi_idx, hf_idx, qf_idx;

    for (idx_int r = 0; r<graph->n_rivers;r++)
    {
                lr = graph->rivers[r].n_int_discret_pts;
                pos = w_idx[r];

                qi_idx = pos;
                hi_idx = n_int_variables+graph->rivers[r].node_start;
                graph->rivers[r].q[0]=w(qi_idx);
                graph->rivers[r].h[0]=w(hi_idx);

                qf_idx = pos+2*lr+1;
                hf_idx = n_int_variables+graph->rivers[r].node_end;
                graph->rivers[r].q[graph->rivers[r].n_int_discret_pts+1]=w(qf_idx);
                graph->rivers[r].h[graph->rivers[r].n_int_discret_pts+1]=w(hf_idx);

                for (idx_int i=1;i<=lr;i++)//ecuaciones del nodo j del i esimo rio
                 {
                    qi_idx = pos + 2*i- 1;
                    hi_idx = pos + 2*i;

                    graph->rivers[r].q[i]=w(qi_idx);
                    graph->rivers[r].h[i]=w(hi_idx);
                 }
                //print results
                //for (idx_int i=1;i<=lr+1;i++)
                //    cout << graph->rivers[r].h[i] << " ";
                //cout << endl;
             }

}
void solver_t::save_w()
{

}
void solver_t::save_Ab()
{

}
void solver_t::qh(idx_int r, double pos_in_river,double * buffer)
{
    idx_int pos;
    idx_int lr;

    lr = graph->rivers[r].n_int_discret_pts;
    pos = w_idx[r];

    if (pos_in_river==1)
    {
        buffer[0]=w[pos+2*lr+1];
        buffer[1]=w[n_int_variables+graph->rivers[r].node_end];
        return;// (rivers[river].q[rivers[river].n_discret_pts -1]);
    }

    else{ if (pos_in_river==0)
    {
        buffer[0]=w[pos];
        buffer[1]=w[n_int_variables+graph->rivers[r].node_start];
        return;// (rivers[river].q[rivers[river].n_discret_pts -1]);
    }
    else
    {
        double frac, node;
        frac = modf (pos_in_river * (graph->rivers[r].n_discret_pts -1 ), &node);

        idx_int qa_idx, qs_idx, ha_idx, hs_idx;

        if (node==0)
        {
            qa_idx = pos;
            ha_idx = n_int_variables+graph->rivers[r].node_start;
        }
        else
        {
            qa_idx = pos + 2*node- 1;
            ha_idx = pos + 2*node;
        }

        if (node==lr)
        {
            qs_idx = pos+2*lr+1;
            hs_idx = n_int_variables+graph->rivers[r].node_end;
        }
        else
        {
            qs_idx = pos + 2*node + 1;
            hs_idx = pos + 2*node + 2;
        }
        buffer[0]=(1-frac)*w[qa_idx]+frac*w[qs_idx];
        buffer[1]=(1-frac)*w[ha_idx]+frac*w[hs_idx];
    }
    }
}

