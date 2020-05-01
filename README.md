# Q_PIX_GEANT4
This is a geant4 geomerty that is a simple box of LAr to produce specific particles for analysis. The particles are output to a text file which can be read into whatever is needed. 

## Installation Geant4 (if needed)

If you do not have brew installed you will need to do that, this will make all the directory’s you will need to hold packages and what not. 
https://brew.sh/
Geant4 will also need xerces-c, and this can be easily installed with home brew but you should check if you have it first on mac its normally located in /usr/local/Cellar if not you can install with 
```bash
brew install xerces-c
```
you will also need cmake you can install it in a similar way 
```bash
brew install cmake
```
you will also need xQuartz with can be installed from 
https://www.xquartz.org/

make a giant directory and grab the latest source of geant from the site. 
https://geant4.web.cern.ch/support/download
move the tar to the giant directory and unpack it 
e.g. /Users/austinmcdonald/Documents/software/GEANT4 which only has the geant4.tar.gz in it. Then unpack the tar file with 
```bash
tar -xzvf geant4.10.06.tar.gz
```
then make a build directory these need to be defined before compiling to your specific ones
e.g 
```bash
mkdir geant4.10.06-build
```
Now before we build it we need to define these paths
```bash
export XERCESC_DIR=/usr/local/Cellar/xerces-c/3.2.2/
export GEANT4_DIR=/Users/austinmcdonald/Documents/software/GEANT4/geant4.10.06
```
Now you should be in the build directory and you can run
cmake -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_OPENGL_X11=ON -DGEANT4_USE_GDML=ON -DXERCESC_ROOT_DIR=$XERCESC_DIR -DCMAKE_INSTALL_PREFIX=$GEANT4_DIR $GEANT4_DIR

then build it with (here the 6 lets it compile on all 6 cores just change it to how many you have [this step takes a bit])
```bash
cmake --build . -j 6
```
and install it
```bash
cmake --build . --target install
```
Once this is completed Geant4 is now installed.
```bash
export DYLD_LIBRARY_PATH=/Users/austinmcdonald/Documents/software/GEANT4/geant4.10.06/lib
```

## Building Q_PIX_GEANT4
Once you have pulled the package and have your Geant4 enviorment setup you should go into the build directory and make the project.
```bash
mkdir Build
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