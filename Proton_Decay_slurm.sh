#!/bin/bash
#SBATCH -J QPIX_Proton_Decay      # A single job name for the array
#SBATCH -n 1                # Number of cores
#SBATCH -N 1                # All cores on one machine
#SBATCH -p node1         # Partition
#SBATCH --mem 1000          # Memory request (Mb)
#SBATCH -t 0-8:00           # Maximum execution time (D-HH:MM)
#SBATCH -o %A_%a.out        # Standard output
#SBATCH -e %A_%a.err        # Standard error


## Job options
JOBNUMBER=${SLURM_ARRAY_TASK_ID}

SEED=`echo "scale = 2;  $JOBNUMBER" | bc`
NAME="Proton_Decay-${SEED}"
OUT="${NAME}.root"

MACRO="Proton_Decay-${NAME}.mac"
OUTFILE="/media/argon/DATA/Ilker/Proton_Decay/output/${OUT}"

#OUTPUTDIR='/n/holystore01/LABS/guenette_lab/Lab/data/QPIX/Supernova_Test/nu_e/'

#MARLEY_json='/n/holystore01/LABS/guenette_lab/Users/amcdonald/Supernova_Qpix/Q_PIX_GEANT4/cfg/marley_config_supernova.js'
#HIST_file='/n/holystore01/LABS/guenette_lab/Users/amcdonald/Supernova_Qpix/nusperbin2d.root'

## making the macro
INPUT_MACRO="/media/argon/DATA/Ilker/Proton_Decay/macros/${MACRO}"
echo $INPUT_MACRO

echo "/control/verbose 1"                             >>${INPUT_MACRO}
echo "/run/verbose 1"                                 >>${INPUT_MACRO}
echo "/tracking/verbose 0"                            >>${INPUT_MACRO}
echo "/Inputs/root_output ${OUTFILE}"                 >>${INPUT_MACRO}
echo "/control/execute  macros/ROOTRead_slurm.macro"  >>${INPUT_MACRO}
echo "/run/initialize"                                >>${INPUT_MACRO}

echo "/run/beamOn 2000"                               >>${INPUT_MACRO}





/home/argon/Projects/Ilker/QPIX_Develop/Build/app/G4_QPIX ${INPUT_MACRO}

#mv "${OUTFILE}" "${OUTPUTDIR}/"