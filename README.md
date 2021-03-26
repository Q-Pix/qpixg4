# Q_PIX_GEANT4
This is a geant4 geomerty that is a simple box of LAr to produce specific particles for analysis. The particles are output to a text file which can be read into whatever is needed. 


## Building Q_PIX_GEANT4
Once you have pulled the package and have your Geant4 enviorment setup you should go into the build directory and make the project.
```bash
cd Build
cmake ../
make
```
This will build the project in the build directory.

## Usage

This can be ran in a batch mode with a mcaro file such as the one included "Template.macro"
where you set the output file, number of events, and particle type 

to see the particle types and how they are generated see "PrimaryGeneration.cpp".
Currently the particles included are 
Ar39
electrons
protons
muons

This can be ran interactivly from the build directory by 
```bash
./app/G4Basic
```
This loads Geant4 and we cna give it the following arguments to run some particles.

```bash
/run/initialize
/tracking/verbose 2
/Inputs/Particle_Type Electron
/run/beamOn 1
```
If you want to save an interactive file you should set the output before runing 
```bash
/Inputs/output_file /path_to_your/tmp.txt
```

Need to add the batch mode information...

## MARLEY
The code can now be ran with marley interfaced 
https://github.com/MARLEY-MC/marley/tree/develop

you will need to clone marley from the develop branch at the moment e.g.
```bash
git clone -b develop https://github.com/MARLEY-MC/marley.git
```
and make a slight tweak to the setup_marley.sh file by adding the flowing to the enf of the file 
```bash
# For building against MARLEY
export CPLUS_INCLUDE_PATH=${CPLUS_INCLUDE_PATH}:${THIS_DIRECTORY}/include
export LIBRARY_PATH=${LIBRARY_PATH}:${THIS_DIRECTORY}/build
```
once that is done you will need to source the marley enviroment before building Q_PIX_GEANT4.

The new additions to the package allow for you to pass a marley config file and it will generate the respective marley events. This also entails a new particle type e.g.

```bash
/run/initialize
/tracking/verbose 2
/Inputs/Particle_Type MARLEY
/Inputs/MARLEY_json /PATH/TO/YOUR/config.js
/run/beamOn 1
```
and the outputs can be save the same way as above.

