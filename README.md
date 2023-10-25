# Q-Pix Geant4 (G4) LArTPC Simulation

## About

This project simulates particle interactions in a single anode plane array (APA) to be used in the Deep Underground Neutrino Experiment (DUNE).
This project has been modified to allow reading of simulated [GENIE](https://hep.ph.liv.ac.uk/~costasa/genie/citing.html) neutrino events.

### Neutrino Beam Particle Simulation

In order to correctly parse large numbers of neutrino events (~1 million) from all different energy ranges (250 MeV to 10+ GeV), additional programs are used to sort the particle hit information created by the Geant4 simulation.
The `sorter` program time sorts each of hits created in a single ROOT file, which corresponds to a single initial particle interaction.

### Radiogenic Particle Simulation
Multiple particle events are combined into a single ROOT file using the `chainsort` program.
This program is primarily used to combine large numbers of radiogenic particle events, as we expect ~707k different particle interactions within an APA every 10 seconds. 
We prevent memory leak issues by simulating each particle interaction individually within the APA, then sorting the time for each particle interaction via `sorter`.
Finally, all of the interactions are combined via the `chainsort` program.
For the user's simplicity, this entire process is handled via `argparse` with the `qpix4mp.py` script.

## System Requirements

This repo is maintained only on MacOS and Unix (Ubuntu 20) operating system.
It requires that that user have installed both ROOT and Geant4 packages, so that CMake (version 3.0+) can find them.

## Please see the Wiki for more details!
https://github.com/Q-Pix/Q_PIX_GEANT4/wiki
