#!/usr/bin/env bash

module purge

module load git/2.17.0-fasrc01
module load cmake/3.17.3-fasrc01
module load gsl/2.6-fasrc01

module load root/6.14.00-fasrc01
module load gcc/9.2.0-fasrc01 geant/4.10.06.p01-fasrc02
source geant4.sh

cd /n/holystore01/LABS/guenette_lab/Users/jh/software/marley/build && source ../setup_marley.sh && cd /path/to/Q_PIX_GEANT4
