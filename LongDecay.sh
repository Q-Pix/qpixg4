#!/bin/bash

outputdir="/home/argon/Projects/Kevin/qpixg4/output/"
outputMacroDir="/home/argon/Projects/Kevin/qpixg4/macros/long_macros/"
prog="/home/argon/Projects/Kevin/qpixg4/build/app/G4_QPIX"
NPROC=10

# clean macrodir
rm "$outputMacroDir""/*"

function makeMacroFile {
	dest="$outputMacroDir""$1"".mac"

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

	# output path
	echo "/Inputs/root_output $outputdir$1_$2.root" >> $dest

	# initialize run
	echo "/run/initialize" >> $dest
	echo "/random/setSeeds $2 707010" >> $dest

	# event ID offset
	echo "/event/offset 0" >> $dest

	# Supernova configs
	echo "/Supernova/Event_Window 10 s" >> $dest
	echo "/Supernova/Event_Cutoff 10 s" >> $dest

  # single macro
  # echo "/Supernova/N_Ar39_Decays 707000" >> $dest
  # echo "/Supernova/N_Ar42_Decays 64" >> $dest
  # echo "/Supernova/N_Bi214_Decays 7000" >> $dest
  # echo "/Supernova/N_Co60_Decays 41" >> $dest
  # echo "/Supernova/N_K40_Decays 12642" >> $dest
  # echo "/Supernova/N_K42_Decays 64" >> $dest
  # echo "/Supernova/N_Kr85_Decays 80500" >> $dest
  # echo "/Supernova/N_Pb214_Decays 7000" >> $dest
  # echo "/Supernova/N_Po210_Decays 5" >> $dest
  # echo "/Supernova/N_Rn222_Decays 27740" >> $dest
	# echo "/run/beamOn 1" >> $dest

  # multiple macros
  echo "/Supernova/$3 1" >> $dest
	echo "/run/beamOn $4" >> $dest
}

file="_Long_Decay"
# single macro
# makeMacroFile "Single"$file 2

# multiple macros
makeMacroFile "Ar39"$file 3 "N_Ar39_Decays" 707000
makeMacroFile "Ar42"$file 3 "N_Ar42_Decays" 64
makeMacroFile "Bi214"$file 3 "N_Bi214_Decays" 7000
makeMacroFile "Co60"$file 3 "N_Co60_Decays" 41
makeMacroFile "K40"$file 3 "N_K40_Decays" 12642
makeMacroFile "K42"$file 3 "N_K42_Decays" 64
makeMacroFile "Kr85"$file 3 "N_Kr85_Decays" 80500
makeMacroFile "Pb214"$file 3 "N_Pb214_Decays" 7000
makeMacroFile "Po210"$file 3 "N_Po210_Decays" 5
makeMacroFile "Rn222"$file 3 "N_Rn222_Decays" 27740

mac_array=( $( ls "$outputMacroDir""/" ) )
## now loop through the above array
for i in "${mac_array[@]}"
do
   echo "Running: $prog $outputMacroDir$i"
   $prog "$outputMacroDir""/""$i" &
done

## Put all cust_func in the background and bash
## would wait until those are completed
## before displaying all done message
wait
echo "All done"
