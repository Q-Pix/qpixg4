#!/bin/bash                                                                                                                                                                                                                            

#SBATCH -J QPIX Marley      # A single job name for the array                                                                                                                                                                          
#SBATCH -n 1                # Number of cores                                                                                                                                                                                          
#SBATCH -N 1                # All cores on one machine                                                                                                                                                                                 
#SBATCH -p guenette         # Partition                                                                                                                                                                                                
#SBATCH --mem 1000          # Memory request (Mb)                                                                                                                                                                                      
#SBATCH -t 0-8:00           # Maximum execution time (D-HH:MM)                                                                                                                                                                         
#SBATCH -o %A_%a.out        # Standard output                                                                                                                                                                                          
#SBATCH -e %A_%a.err        # Standard error                                                                                                                                                                                           

# source the G4 stuff                                                                                                                                                                                                                  
source /n/holystore01/LABS/guenette_lab/Users/amcdonald/Supernova_Qpix/johnny.sh
# setup marley                                                                                                                                                                                                                         
source /n/holystore01/LABS/guenette_lab/Users/amcdonald/Supernova_Qpix/marley/setup_marley.sh

## Job options                                                                                                                                                                                                                         
JOBNUMBER=${SLURM_ARRAY_TASK_ID}
PRESSURE=39
SEED=`echo "scale = 2; $PRESSURE * $JOBNUMBER" | bc`
NAME="Nu_e-${SEED}"
OUT="${NAME}.root"

MACRO="MACRO-${NAME}.mac"
OUTFILE="/n/holyscratch01/guenette_lab/${OUT}"

OUTPUTDIR='/n/holystore01/LABS/guenette_lab/Lab/data/QPIX/Supernova_Test/nu_e/'

MARLEY_json='/n/holystore01/LABS/guenette_lab/Users/amcdonald/Supernova_Qpix/Q_PIX_GEANT4/cfg/marley_config_supernova.js'
HIST_file='/n/holystore01/LABS/guenette_lab/Users/amcdonald/Supernova_Qpix/nusperbin2d.root'

## making the macro                                                                                                                                                                                                                    
INPUT_MACRO="/n/holyscratch01/guenette_lab/${MACRO}"
echo $INPUT_MACRO

echo "/control/verbose 1"                             >>${INPUT_MACRO}
echo "/run/verbose 1"                                 >>${INPUT_MACRO}
echo "/tracking/verbose 0"                            >>${INPUT_MACRO}

echo "/Inputs/Particle_Type MARLEY"                   >>${INPUT_MACRO}
echo "/Inputs/MARLEY_json ${MARLEY_json}"             >>${INPUT_MACRO}

echo "/supernova/timing/on         true"              >>${INPUT_MACRO}
echo "/supernova/timing/input_file ${HIST_file}"      >>${INPUT_MACRO}
echo "/supernova/timing/th2_name   nusperbin2d_nue"   >>${INPUT_MACRO}

echo "/Inputs/root_output ${OUTFILE}"                 >>${INPUT_MACRO}
echo "/run/initialize"                                >>${INPUT_MACRO}
echo "/random/setSeeds 0 ${SEED}"                     >>${INPUT_MACRO}

echo "/Supernova/Event_Cutoff 10 s"                   >>${INPUT_MACRO}
echo "/run/beamOn 1000"                               >>${INPUT_MACRO}



/n/holystore01/LABS/guenette_lab/Users/amcdonald/Supernova_Qpix/Q_PIX_GEANT4/Build/app/G4Basic ${INPUT_MACRO} 

mv "${OUTFILE}" "${OUTPUTDIR}/"
