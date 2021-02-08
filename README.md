# Setting up the environment

Open up `setup.sh` and change find the line 
```bash
cd /n/holystore01/LABS/guenette_lab/Users/jh/software/marley/build && source ../setup_marley.sh && cd /path/to/Q_PIX_GEANT4
```
change `/path/to/Q_PIX_GEANT4` to wherever your own path to the `Q_PIX_GEANT4`
repository is located.  Once that is done, simply run
```bash
source /path/to/Q_PIX_GEANT4
```
to set up the environment.

# Compiling the code

To compile the code, simply do the following
```bash
cd build
cmake ..
make
```
This will compile the project code in the `build` directory.

# Running the program

Go into the `gxe` directory, and run the following command
```bash
../build/app/G4Basic electron.mac
```
A ROOT file should appear in the `gxe/output` directory.
