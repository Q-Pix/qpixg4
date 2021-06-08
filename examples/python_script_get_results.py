import ROOT
import csv
import numpy as np
import matplotlib
matplotlib.use('Agg') # Must be before importing matplotlib.pyplot or pylab!
from mpl_toolkits import mplot3d
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

#defining detector range
detector_size_x=[0,230]
detector_size_y=[0,360]
detector_size_z=[0,600]

## For the TEST
counter = 0

if __name__ == "__main__":

    # Read in the root file
   inFileName = "/n/home04/sruppert/Q_PIX_GEANT4/gxe/output/electron.root"
   inFile = ROOT.TFile.Open(inFileName, "READ")

   # Read the trees in the root file
   tree1 = inFile.Get("event_tree")

   # Loop over events in the tree
   for event in range(0, tree1.GetEntries()):
       tree1.GetEntry(event)

       #if not first event, clear variables
       if event > 0:
           del (number_particles, number_hits, particle_track_id, particle_parent_track_id,
                    particle_initial_x, particle_initial_y, particle_initial_z,
                    particle_initial_px, particle_initial_py, particle_initial_pz,
                    hit_track_id, hit_start_x, hit_start_y, hit_start_z, hit_start_t,
                    hit_end_x, hit_end_y, hit_end_z, hit_end_t)

       # get particle and hit branches
       number_particles= getattr (tree1,"number_particles")
       number_hits= getattr (tree1,"number_hits")
       particle_track_id= getattr (tree1,"particle_track_id")
       particle_parent_track_id= getattr (tree1,"particle_parent_track_id")
       particle_initial_x= getattr (tree1,"particle_initial_x")
       particle_initial_y= getattr (tree1,"particle_initial_y")
       particle_initial_z= getattr (tree1,"particle_initial_z")
       particle_initial_px= getattr (tree1,"particle_initial_px")
       particle_initial_py=getattr (tree1,"particle_initial_py")
       particle_initial_pz=getattr (tree1,"particle_initial_pz")
       hit_track_id=getattr (tree1,"hit_track_id")
       hit_start_x=getattr (tree1,"hit_start_x")
       hit_start_y=getattr (tree1,"hit_start_y")
       hit_start_z=getattr (tree1,"hit_start_z")
       hit_start_t=getattr (tree1,"hit_start_t")
       hit_end_x=getattr (tree1,"hit_end_x")
       hit_end_y=getattr (tree1,"hit_end_y")
       hit_end_z=getattr (tree1,"hit_end_z")
       hit_end_t=getattr (tree1,"hit_end_t")
       particle_initial_energy=getattr (tree1, "particle_initial_energy")

       ## loop over all particles to get to the primary particle(s)
       for i in range(number_particles):
           # if primary particle
           if particle_parent_track_id[i]==0:
               electron_track_id = particle_track_id[i]

               # these are the values we are trying to reconstruct (inital position and momentum)
               electron_start_position = [particle_initial_x[i], particle_initial_y[i], particle_initial_z[i]]
               electron_start_momentum = [particle_initial_px[i], particle_initial_py[i], particle_initial_pz[i]]

               ## loop over all hits to get all hits associated with the primary electron_track_id
               hits_start = []
               hits_end = []
               k = 0
               for j in range (number_hits):
                   if hit_track_id[j] == electron_track_id:
                       # get position x,y,z of hit
                       # hits_start.append([hit_start_x[j], hit_start_y[j], hit_start_z[j], hit_start_t[j]])
                       hits_end.append([hit_end_x[j], hit_end_y[j], hit_end_z[j], hit_end_t[j]])
                       k += 1
               hits_end = np.array(hits_end)

               ## TEST TEST Test
               print('electron hits (x,y,z,t):', hits_end)
               #hits_end_x = np.array(hit_end_x)
               #print('all hit_end_x:', hits_end_x)
               #print('electron_track_id:', electron_track_id)
               #hits_track_id = np.array(hit_track_id)
               #print('hit_track_id:', hits_track_id)

               ### If there are electron hits, make graphs
               if len(hits_end) > 0:
                   counter += 1
                   ## Make graphs of hits
                   # define x,y,z data
                   x = hits_end[1,0]
                   y = hits_end[1,1]
                   z = hits_end[1,2]
                   

                   # true trajectory
                   electron_vacuum_point2 = np.add(electron_start_position, np.multiply(electron_start_momentum,0.075))
                   true_x=[electron_start_position[0], electron_vacuum_point2[0]]
                   true_y=[electron_start_position[1], electron_vacuum_point2[1]]
                   true_z=[electron_start_position[2], electron_vacuum_point2[2]]

                   #3D graph
                   fig1 = plt.figure()
                   fig1.suptitle('Electron hits 3d')
                   ax1 = fig1.add_subplot(111, projection='3d')
                   ax1.set_xlabel('$X$')
                   ax1.set_ylabel('$Y$')
                   ax1.set_zlabel('$Z$')
                   #ax1.set_xlim(detector_size_x)
                   #ax1.set_ylim(detector_size_y)
                   #ax1.set_zlim(detector_size_z)
                   ax1.set_aspect('auto', adjustable='box')
                   ax1.scatter(x,y,z,s=1, label='hits')
                   ax1.scatter(electron_start_position[0],electron_start_position[1],electron_start_position[2],s=2, color='red')
                   ax1.plot(true_x, true_y, true_z, linewidth=0.5, color='red', label='true trajectory')
                   leg = ax1.legend();

                   fig1.savefig('electron_path_3d_'+ str(event) + '.png', bbox_inches='tight')   # Save graph as a png, to your current directory

                   #2D graphs
                   fig2, axs = plt.subplots(2,2)
                   fig2.tight_layout()
                   fig2.suptitle('Electron hits 2d', y=1.08)
                   #fig2.title(subtitle_string)
                   rect1 = plt.Rectangle((electron_start_position[0], electron_start_position[1]), 1, 1, edgecolor='red', alpha=1, facecolor='none', fill=False)
                   rect2 = plt.Rectangle((electron_start_position[0],electron_start_position[2]), 1, 1, edgecolor='red', alpha=1, facecolor='none', fill=False)
                   rect3 = plt.Rectangle((electron_start_position[1],electron_start_position[2]), 1, 1, edgecolor='red', alpha=1, facecolor='none', fill=False)

                   # x-y
                   axs[0,0].set_xlabel('$X$')
                   axs[0,0].set_ylabel('$Y$')
                   axs[0,0].scatter(electron_start_position[0],electron_start_position[1],s=4, color='red', label='true start')
                   #axs[0,0].set_xlim(detector_size_x)
                   #axs[0,0].set_ylim(detector_size_y)
                   axs[0,0].set(adjustable='box-forced', aspect='equal')
                   axs[0,0].add_patch(rect1)

                   # x-z
                   axs[0,1].set_xlabel('$X$')
                   axs[0,1].set_ylabel('$Z$')
                   axs[0,1].scatter(electron_start_position[0],electron_start_position[2],s=4, color='red', label='true start')
                   #axs[0,1].set_xlim(detector_size_x)
                   #axs[0,1].set_ylim(detector_size_z)
                   axs[0,1].set_aspect('equal', adjustable='box')
                   axs[0,1].add_patch(rect2)

                   # y-z
                   axs[1,0].set_xlabel('$Y$')
                   axs[1,0].set_ylabel('$Z$')
                   axs[1,0].scatter(electron_start_position[1],electron_start_position[2],s=4, color='red', label='true start')
                   #axs[1,0].set_xlim(detector_size_y)
                   #axs[1,0].set_ylim(detector_size_z)
                   axs[1,0].set_aspect('equal', adjustable='box')
                   axs[1,0].add_patch(rect3)

                   # populate the graph with scatter points
                   axs[0,0].scatter(x,y,s=1, label='hits')
                   axs[0,1].scatter(x,z,s=1, label='hits')
                   axs[1,0].scatter(y,z,s=1, label='hits')
                   # add true trajectory
                   axs[0,0].plot(true_x, true_y, linewidth=0.5, color='red', label='true trajectory')
                   axs[0,1].plot(true_x, true_z, linewidth=0.5, color='red', label='true trajectory')
                   axs[1,0].plot(true_y, true_z, linewidth=0.5, color='red', label='true trajectory')

                   fig2.delaxes(axs[1,1])

                   # create legend
                   handles, labels = axs[0,0].get_legend_handles_labels()
                   fig2.legend(handles, labels,loc = 'best')

                   fig2.savefig('electron_path_2d_'+ str(event) + '.png', bbox_inches='tight')

                   # Test: Return the original energy of the electron:
                   print(str(particle_initial_energy[i]))
print('runs with one or more hits:', counter)
