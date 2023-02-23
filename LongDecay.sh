#!/bin/bash

outputdir="/home/argon/Projects/Kevin/qpixg4/output/"
outputMacroDir="/home/argon/Projects/Kevin/qpixg4/macros/long_macros/"
NPROC=10

function makeMacroFile {
	touch "$outputMacroDir""$1"".mac"
	dest="$outputMacroDir""$1"".mac"

	echo "making file: $dest"

	# set verbosity
	echo "/control/verbose 1" >> $dest
	echo "/run/verbose 1" >> $dest
	echo "/tracking/verbose 0" >> $dest

	# # configure supernova
	echo "/Inputs/Particle_Type SUPERNOVA" >> $dest

	# # output path
	echo "/Inputs/root_output $outputdir$1_$2.root" >> $dest

	# # initialize run
	echo "/run/initialize" >> $dest
	echo "/random/setSeeds $2 707010" >> $dest

	# # event ID offset
	echo "/event/offset 0" >> $dest

	# # Supernova configs
	echo "/Supernova/Event_Window 100000 s" >> $dest
	echo "/Supernova/Event_Cutoff 100000 s" >> $dest

	echo "/Supernova/N_Ar39_Decays 1" >> $dest

	# # run
	echo "/run/beamOn 707000" >> $dest
}

prog="/home/argon/Projects/Kevin/qpixg4/build/app/G4_QPIX"
function runG4 {

}


for i in {1..10}
do
	file="Ar39_seed$i"
	makeMacroFile $file $i & # Put a function in the background
	makeMacroFile $file $i & # Put a function in the background

done

## Put all cust_func in the background and bash
## would wait until those are completed
## before displaying all done message
wait
echo "All done"
