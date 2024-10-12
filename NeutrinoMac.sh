#!/bin/bash

outputMacroDir="/home/argon/Projects/Kevin/qpixg4/macros/neutrino_macros/"
prog="/home/argon/Projects/Kevin/qpixg4/build/app/G4_QPIX"

# x pos
if [ -z $1 ]
  then
    echo "No arguments for x position received.."
    exit 1
  else
    xpos=$1
fi

# y pos
if [ -z $2 ]
  then
    echo "No arguments for y position received.."
    exit 1
  else
    ypos=$2
fi

# z pos
if [ -z $3 ]
  then
    echo "No arguments for z position received.."
  else
    zpos=$3
fi

# seed
if [ -z $4 ]
  then
    echo "No arguments for seed received.."
  else
    seed=$4
fi

# input file
if [ -z $5 ]
  then
    echo "No arguments for input file received.."
  else
    input_file=$5
fi

# find the outputdir from controlling python program
if [ -z $6 ]
  then
    echo "No argument selected for outputdir, error!"
    exit 1
  else
    outputdir=$6
fi

# options 1-5 select create the angle here
if [ -z $7 ]
  then
    echo "no argument select for angle"
    exit 1
  else
    if [ "$7" == "1" ];  # theta = 0
    then
      xaxis=1
      yaxis=0
      zaxis=0
      t=$7
    elif [ "$7" == "2" ];  # theta = +2deg
    then
      xaxis=10000
      yaxis=0
      zaxis=349
      t=$7
    elif [ "$7" == "3" ];  # theta = -2deg
    then
      xaxis=10000
      yaxis=0
      zaxis=-349
      t=$7
    elif [ "$7" == "4" ];  # theta = +90deg
    then
      xaxis=0
      yaxis=0
      zaxis=1
      t=$7
    elif [ "$7" == "5" ];  # theta = -90deg
    then
      xaxis=0
      yaxis=0
      zaxis=-1
      t=$7
    elif [ "$7" == "6" ];  # theta = y dir
    then
      xaxis=0
      yaxis=1
      zaxis=0
      t=$7
    else
      echo "incorrect argument supplied for angle!"
      exit 1
    fi
fi

# must select an event
if [ -z $8 ]
  then
    echo "No argument selected for event, error!"
    exit 1
  else
    nEvt=$8
fi

# pdg
if [ -z $9 ]
  then
    echo "No argument selected for pdg, error!"
    exit 1
  else
    fsPdg=$9
fi

# energy
if [ -z ${10} ]
  then
    echo "No argument selected for energy, error!"
    exit 1
  else
    fsEnergy=${10}
fi

# fhc
if [ -z ${11} ]
  then
    echo "No argument selected for FHC, error!"
    exit 1
  else
    fsFHC=${11}
fi

function makeMacroFile {

  # make sure input file exists
  if ! test -f "$input_file"; then
    echo 'could not find input file!'
    exit 1
  fi

  # make the output macro based on the input file
  dest=$(echo "$input_file" | cut -d '/' -f 7 | cut -d '.' -f 1 )
  name="$dest""pdg-$fsPdg""_E-$fsEnergy""_evt-$nEvt""_FHC-$fsFHC""_z-$zpos""_seed-$seed""_t-""$t"
  dest="$outputMacroDir""$name"".mac"
  output_file="$outputdir""$name"".root"

  if test -f "$dest"; then
    rm "$dest"
  fi
  touch "$dest"

  echo "/control/verbose 1" >> $dest
  echo "/run/verbose 1" >> $dest
  echo "/tracking/verbose 0" >> $dest
  echo "/inputs/output_file $output_file" >> $dest
  echo "/inputs/particle_type ProtonDecay" >> $dest
  echo "/inputs/TreeName tree" >> $dest
  echo "/inputs/ReadFrom_Root_Path $input_file" >> $dest
  ## meta data inputs that go to the ROOTManager
  echo "/inputs/vertex_x $xpos cm" >> $dest
  echo "/inputs/vertex_y $ypos cm" >> $dest
  echo "/inputs/vertex_z $zpos cm" >> $dest
  echo "/inputs/axis_x $xaxis" >> $dest
  echo "/inputs/axis_y $yaxis" >> $dest
  echo "/inputs/axis_z $zaxis" >> $dest
  # new inputs
  echo "/inputs/nEvt $nEvt" >> $dest
  echo "/inputs/fsPdg $fsPdg" >> $dest
  echo "/inputs/fsEnergy $fsEnergy" >> $dest
  echo "/inputs/fsFHC  $fsFHC" >> $dest
  echo "/inputs/fsRun 0" >> $dest
  # old inputs
  echo "/inputs/PrintParticleInfo false" >> $dest
  echo "/run/initialize" >> $dest
  echo "/random/setSeeds 0 $seed" >> $dest
  echo "/run/beamOn 1" >> $dest
}

makeMacroFile