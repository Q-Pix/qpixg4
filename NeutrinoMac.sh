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

function makeMacroFile {

  # make sure input file exists
  if ! test -f "$input_file"; then
    echo 'could not find input file!'
    exit 1
  fi

  # make the output macro based on the input file
  dest=$(echo "$input_file" | cut -d '/' -f 7 | cut -d '.' -f 1 )
  dest="$outputMacroDir""$dest""_x-$xpos""_y-$ypos""_z-$zpos""_seed-$seed""_input.mac"

	echo "making file: $dest"
  if test -f "$dest"; then
    rm "$dest"
  fi
  touch "$dest"

  echo "/control/verbose 1" >> $dest
  echo "/run/verbose 1" >> $dest
  echo "/tracking/verbose 0" >> $dest
  echo "/Inputs/root_output $output_file" >> $dest
  echo "/Inputs/Particle_Type ProtonDecay" >> $dest
  echo "/Inputs/TreeName tree" >> $dest
  echo "/Inputs/ReadFrom_Root_Path $input_file" >> $dest
  echo "/Inputs/vertex_x $xpos" >> $dest
  echo "/Inputs/vertex_y $ypos" >> $dest
  echo "/Inputs/vertex_z $zpos" >> $dest
  echo "/Inputs/PrintParticleInfo false" >> $dest
  echo "/run/initialize" >> $dest
  echo "/random/setSeeds 0 $seed" >> $dest
  echo "/run/beamOn 1" >> $dest
}

makeMacroFile
echo "All done"