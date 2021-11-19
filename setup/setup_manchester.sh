#!/usr/bin/env bash

source /cvmfs/larsoft.opensciencegrid.org/products/setup

setup git
setup mrb
setup cmake  v3_20_0
setup boost  v1_75_0      -q e20:prof
setup root   v6_22_08d    -q e20:p392:prof
setup geant4 v4_10_6_p01d -q e20:prof
setup marley v1_2_0e      -q e20:prof

cd ${MARLEY}/marley-`marley-config --version`/build && source ${MARLEY}/marley-`marley-config --version`/setup_marley.sh && cd ${OLDPWD}
