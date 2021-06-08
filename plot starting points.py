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
start_positions_x = []
start_positions_y = []
start_positions_z = []

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
                    particle_initial_x, particle_initial_y, particle_initial_z, particle_initial_energy)

       # get particle and hit branches
       number_particles= getattr (tree1,"number_particles")
       number_hits= getattr (tree1,"number_hits")
       particle_track_id= getattr (tree1,"particle_track_id")
       particle_parent_track_id= getattr (tree1,"particle_parent_track_id")
       particle_initial_x= getattr (tree1,"particle_initial_x")
       particle_initial_y= getattr (tree1,"particle_initial_y")
       particle_initial_z= getattr (tree1,"particle_initial_z")
       particle_initial_energy=getattr (tree1, "particle_initial_energy")

       ## loop over all particles to get to the primary particle(s)
       for i in range(number_particles):
           # if primary particle
           if particle_parent_track_id[i]==0:
               electron_track_id = particle_track_id[i]

               # these are the values we are trying to reconstruct (inital position and momentum)
                    #electron_start_position = [particle_initial_x[i], particle_initial_y[i], particle_initial_z[i]]
               start_positions_x.append(particle_initial_x[i])
               start_positions_y.append(particle_initial_y[i])
               start_positions_z.append(particle_initial_z[i])

   ## Make graphs of hits
   #print(start_positions_x, start_positions_y, start_positions_z)
   # define x,y,z data
   x = start_positions_x
   y = start_positions_y
   z = start_positions_z

   #3D graph
   fig1 = plt.figure()
   fig1.suptitle('Electron start positions - 100 runs')
   ax1 = fig1.add_subplot(111, projection='3d')
   ax1.set_xlabel('$X$')
   ax1.set_ylabel('$Y$')
   ax1.set_zlabel('$Z$')
   #ax1.set_xlim(detector_size_x)
   #ax1.set_ylim(detector_size_y)
   #ax1.set_zlim(detector_size_z)
   ax1.set_aspect('auto', adjustable='box')
   ax1.scatter(x,y,z,s=1, label='hits')
   leg = ax1.legend();

   electron_energy = particle_initial_energy[i]
   fig1.savefig('electron_start_positions_3d_'+ str(round(electron_energy,2)) + 'MeV_nottoscale.png', bbox_inches='tight')   # Save graph as a png, to your current directory

   #2D graphs
   fig2, axs = plt.subplots(2,2)
   fig2.tight_layout()
   fig2.suptitle('Electron start positions 2d - 100 runs', y=1.08)

   # x-y
   axs[0,0].set_xlabel('$X$')
   axs[0,0].set_ylabel('$Y$')
   #axs[0,0].set(adjustable='box-forced', aspect='equal')

   # x-z
   axs[0,1].set_xlabel('$X$')
   axs[0,1].set_ylabel('$Z$')
   #axs[0,1].set(adjustable='box-forced', aspect='equal')

   # y-z
   axs[1,0].set_xlabel('$Y$')
   axs[1,0].set_ylabel('$Z$')
   #axs[1,0].set(adjustable='box-forced', aspect='equal')

   # populate the graph with scatter points
   axs[0,0].scatter(x,y,s=1, label='hits')
   axs[0,1].scatter(x,z,s=1, label='hits')
   axs[1,0].scatter(y,z,s=1, label='hits')

   fig2.delaxes(axs[1,1])

   # create legend
   handles, labels = axs[0,0].get_legend_handles_labels()
   fig2.legend(handles, labels,loc = 'best')

   fig2.savefig('electron_start_positions_2d_'+ str(round(electron_energy,2)) + 'MeV_nottoscale.png', bbox_inches='tight')
   print(electron_energy)
