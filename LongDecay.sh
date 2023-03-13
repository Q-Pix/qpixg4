#!/bin/bash

outputMacroDir="/home/argon/Projects/Kevin/qpixg4/macros/long_macros/"
prog="/home/argon/Projects/Kevin/qpixg4/build/app/G4_QPIX"

if [ -z $1 ]
  then
    echo "No arguments for time received.."
    exit 1
fi

if [ -z $2 ]
  then
    echo "No arguments for number of cores received.."
    exit 1
fi

# base seed
if [ -z $3 ]
  then
    echo "No arguments for base seed of cores received.. using default"
    baseseed=0
  else
    baseseed=$3
fi

# find the outputdir from controlling python program
if [ -z $4 ]
  then
    echo "No argument selected for outputdir, error!"
    exit 1
    # deprecated output
    # outputdir="/home/argon/Projects/Kevin/qpixg4/output/"
  else
    # baseseed=$3
    outputdir=$4
fi

# required args to make the geant data
time=$1
maxCores=$2

# clean macrodir
rm -rf "$outputMacroDir"
mkdir $outputMacroDir

function makeMacroFile {
	dest="$outputMacroDir""$1""_$2.mac"

	echo "making file: $dest"
  if test -f "$dest"; then
    rm "$dest"
  fi
  touch "$dest"

	# set verbosity
	echo "/control/verbose 1" >> $dest
	echo "/run/verbose 1" >> $dest
	echo "/tracking/verbose 0" >> $dest

	# configure supernova
	echo "/Inputs/Particle_Type SUPERNOVA" >> $dest
	echo "/Inputs/RadioParticleID $5" >> $dest

	# output path
	echo "/Inputs/root_output $outputdir$1_$2.root" >> $dest

	# initialize run, with a seed
	echo "/run/initialize" >> $dest
	echo "/random/setSeeds 0 $2" >> $dest

	# event ID offset
	echo "/event/offset 0" >> $dest

	# Supernova configs
	echo "/Supernova/Event_Window $time s" >> $dest
	echo "/Supernova/Event_Cutoff $time s" >> $dest

  # multiple macros
  echo "/Supernova/$3 1" >> $dest
	echo "/run/beamOn $4" >> $dest
}

# we need time/10 / 10s files per core
maxTime=$(($time/10))
for ((j=0; j<$maxCores; j++))
do
  for ((i=1; i<=$maxTime; i++ ))
  do
    seed=$(($maxTime*$j+$i+$baseseed))
    echo "making macro.. $i"
    makeMacroFile "Ar39"$file"_core-$j" "$seed" "N_Ar39_Decays" 707000 1
    makeMacroFile "Ar42"$file"_core-$j" "$seed" "N_Ar42_Decays" 64 2
    makeMacroFile "Bi214"$file"_core-$j" "$seed" "N_Bi214_Decays" 7000 3
    makeMacroFile "Co60"$file"_core-$j" "$seed" "N_Co60_Decays" 41 4
    makeMacroFile "K40"$file"_core-$j" "$seed" "N_K40_Decays" 12642 5
    makeMacroFile "K42"$file"_core-$j" "$seed" "N_K42_Decays" 64 6
    makeMacroFile "Kr85"$file"_core-$j" "$seed" "N_Kr85_Decays" 80500 7
    makeMacroFile "Pb214"$file"_core-$j" "$seed" "N_Pb214_Decays" 7000 8
    makeMacroFile "Po210"$file"_core-$j" "$seed" "N_Po210_Decays" 5 9
    makeMacroFile "Rn222"$file"_core-$j" "$seed" "N_Rn222_Decays" 27740 10
  done
done

echo "All done"
