# -*- coding: utf-8 -*-
"""
Created on Mon Jun 10 00:45:13 2013

@author: miles
"""

import os
import time
import csv
#
#tic2 = time.time()
#
######################################################################
##PARAMETERS
#dx = 0.2
#g = 9.8
#cm = 0.005
#dt = 0.1
#theta = 0.75
#nt = 100
#n_nodes = 18
#n_rivers = 22
#
#parameters = array((n_rivers,n_nodes,dx,theta,g,cm,dt,nt),ndmin=2);
#
#fh = file('ioFiles/parameters.in','w')
#fh.write('#Rivers_Nodes_dx_theta_g_cm_dt\n')
#fh.close()
#
#fh = file('ioFiles/parameters.in','a')
#savetxt(fh,parameters, fmt='%g', delimiter=' ')
#fh.close()
#
######################################################################
##RIVERS
##Name	  Orig	Dest	Length	Type	Discret_pts(internals)
#
#rivers = {}
#rivers[ 0] = ['Parana'      ,0 ,1 ,1,1,3]
#rivers[ 1] = ['Parana'      ,1 ,2 ,1,1,3]
#rivers[ 2] = ['Parana'      ,2 ,3 ,1,1,3]
#rivers[ 3] = ['Parana'      ,3 ,4 ,1,1,3]
#rivers[ 4] = ['Parana'      ,4 ,5 ,1,1,3]
#rivers[ 5] = ['Lujan'       ,6 ,7 ,1,4,3]
#rivers[ 6] = ['Lujan'       ,7 ,8 ,1,4,3]
#rivers[ 7] = ['Lujan'       ,8 ,9 ,1,4,3]
#rivers[ 8] = ['Lujan'       ,9 ,10,1,4,3]
#rivers[ 9] = ['Lujan'       ,10,11,1,4,3]
#rivers[10] = ['Lujan'       ,11,12,1,4,3]
#rivers[11] = ['Reconq'      ,13,9 ,1,4,3]
#rivers[12] = ['Arias'       ,1 ,7 ,1,4,3]
#rivers[13] = ['Carapachay'  ,2 ,14,1,4,3]
#rivers[14] = ['Carapachay'  ,14,8 ,1,4,3]
#rivers[15] = ['Espera'      ,14,15,1,4,3]
#rivers[16] = ['Capitan'     ,3 ,16,1,3,3]
#rivers[17] = ['Sarmiento'   ,16,15,1,3,3]
#rivers[18] = ['Sarmiento'   ,15,10,1,3,3]
#rivers[19] = ['SanAntonio'  ,16,17,1,2,3]
#rivers[20] = ['Honda'       ,4 ,17,1,2,3]
#rivers[21] = ['Vinculacion' ,17,12,1,2,3]
#
#fh = file('ioFiles/rivers.in','w')
#fh.write('#Name	Orig	Dest	Length	Type	Discret_pts(internals)\n')
#fh.close()
#fh = file('ioFiles/rivers.in','a')
#writer = csv.writer(fh,delimiter='\t')
#for key, value in rivers.items():
#   writer.writerow(value)
#fh.close()
#
######################################################################
##BOUNDARY CONDITIONS
#
#n_boundary_nodes = [5]
#boundary_nodes = [ 0 , 5 , 6 , 12 , 13 ]
#
#single_nodes_info = array([n_boundary_nodes + boundary_nodes] , ndmin=2)
#
#boundary_conditions = zeros((5,nt))
#boundary_conditions[0,0:5]=0.1
#
#fh = file('ioFiles/boundary_conditions.in','w')
#savetxt(fh,single_nodes_info, fmt='%g', delimiter=' ')
#fh.close()
#
#fh = file('ioFiles/boundary_conditions.in','a')
#savetxt(fh,boundary_conditions, fmt='%g', delimiter=' ')
#fh.close()
#
######################################################################
##ELEVATION
#
#elevation = {}
#fh = file('ioFiles/elevation.in','w')
#fh.close()
#fh = file('ioFiles/elevation.in','a')
#for i in range(n_rivers):
#    elevation[i] = ones((1,rivers[i][5]+2))
#    savetxt(fh,elevation[i], fmt='%g', delimiter=' ')    
#
#fh.close()


model = '2parallels'

lines = [line.strip() for line in open(model+'/ModelData.in')]

dd = {}

for s in lines:
    aux = s.split('=')
    aux2 = aux[1].strip()
    if aux2.isdigit():
        dd[aux[0].strip()] = float(aux2)
    else:
        dd[aux[0].strip()] = aux2
    

#####################################################################
#COMPUTATION

#os.system('make')

#tic = time.time()
#os.system('./SWD_debug')
#toc = time.time()
#print "Tiempo demorado", "%f segundos" % (toc - tic,)

#####################################################################
#READOUT

qh = fromfile(model+'/output/locations.out', dtype=float64)
qh = qh.reshape((dd['nt']+1,dd['n_locations']*2))

#####################################################################
#GRAPHICS

close()
figure()

subplot(2,1,1)
plot(qh[:,0])
plot(qh[:,2])

subplot(2,1,2)
plot(qh[:,1])
plot(qh[:,3])

#imshow(hq,aspect='auto',interpolation='nearest')
#colorbar()
