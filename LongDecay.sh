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
	echo "/random/setSeeds 0 $2" >> $dest

	# event ID offset
	echo "/event/offset 0" >> $dest

	# Supernova configs
	echo "/Supernova/Event_Window 1000 s" >> $dest
	echo "/Supernova/Event_Cutoff 1000 s" >> $dest

  # multiple macros
  echo "/Supernova/$3 1" >> $dest
	echo "/run/beamOn $4" >> $dest
}

# multiple macros
for i in {1..10} 
do
  echo "making macro.. $i"
  makeMacroFile "Ar39"$file"_$i" "$i" "N_Ar39_Decays" 70700000
  makeMacroFile "Ar42"$file"_$i" "$i" "N_Ar42_Decays" 6400
  makeMacroFile "Bi214"$file"_$i" "$i" "N_Bi214_Decays" 700000
  makeMacroFile "Co60"$file"_$i" "$i" "N_Co60_Decays" 4100
  makeMacroFile "K40"$file"_$i" "$i" "N_K40_Decays" 1264200
  makeMacroFile "K42"$file"_$i" "$i" "N_K42_Decays" 6400
  makeMacroFile "Kr85"$file"_$i" "$i" "N_Kr85_Decays" 8050000
  makeMacroFile "Pb214"$file"_$i" "$i" "N_Pb214_Decays" 700000
  makeMacroFile "Po210"$file"_$i" "$i" "N_Po210_Decays" 500
  makeMacroFile "Rn222"$file"_$i" "$i" "N_Rn222_Decays" 2774000
done

# mac_array=( $( ls "$outputMacroDir""/" ) )
## now loop through the above array
# for i in "${mac_array[@]}"
# do
#    echo "Running: $prog $outputMacroDir$i"
#    $prog "$outputMacroDir""/""$i" &
# done

## Put all cust_func in the background and bash
## would wait until those are completed
## before displaying all done message
wait
echo "All done"
