## Please see the Wiki for details!
https://github.com/Q-Pix/Q_PIX_GEANT4/wiki

Code works with GEANT4 v1.11.0 and ROOT v6.30.02

-------------------------------------------------------------------------------
  GVS: 23 April 2024
       ATTENTION! USE THE FOLLOWING COMMAND LINES WHEN 
       RUNNING QPIXG4 FOR NEUTRON BACKGROUND STUDIES:
       
       cd PathToQPIXG4directory/qpixg4
       source run_neutron_background.sh
------------------------------------------------------------------------------
 
## Macros for running Neutron Background Simulation 
1) macros/neutron_surf.mac --> Simulates neutron from DUNE cave and concrete. Modify "/run/beamOn 100000" command on line 342 to change number of simulated neutrons.
Outputs: output/neutron.root
         output/photon_ncap_events.txt --> Photons from neutron capture. To be used by the macro 2) after being properly formatted by WritePhotons.py script
         
--------------------------------------------------------------------------------------------
Neutron Spectrum from Guilherme Ruiz Ferreira (U. of Manchester)
Email: guilherme.ruizferreira@postgrad.manchester.ac.uk
Total neutron rates considering one DUNE cave (2 in total) with dimensions of 145mx20mx28m
Total fluence of 277 neutrons/second
Differencial fluence of 0.01842 neutrons/second/m^2
--------------------------------------------------------------------------------------------

2) macros/Template_Neutron_Background_forRun.mac --> Continuation of simulation done by macro 1). Simulates photons from neutron capture. It gives the products of the photon's electromagnetic interactions inside Liquid Argon.
Outputs: output/electron_NB.root
         output/electron_ncap.txt --> Electrons from neutron capture photon's electromagnetic interactions. Mimic products from electron neutrino interactions in the detector. 
         
## Interactive mode is also available
run "./app/G4_QPIX"

Copy the content of the desired macro in the User Interactive Session and run few events with command "run/beamOn 5"
