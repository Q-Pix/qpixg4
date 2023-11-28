#!/usr/bin/env bash

source /cvmfs/larsoft.opensciencegrid.org/products/setup

setup git
setup mrb
setup cmake  v3_20_0
setup boost  v1_75_0      -q e20:prof
setup root   v6_22_08d    -q e20:p392:prof
setup marley v1_2_0e      -q e20:prof

pushd ${MARLEY}/marley-`marley-config --version`/build
source ${MARLEY}/marley-`marley-config --version`/setup_marley.sh
popd

pushd /cvmfs/geant4.cern.ch/geant4/10.7.p04a/x86_64-centos7-gcc9-optdeb-MT/share/Geant4-10.7.4/geant4make/
source geant4make.sh
popd
