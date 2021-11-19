#!/usr/bin/env bash                                                             
                                                                                
source /n/holystore01/LABS/guenette_lab/Lab/software/next/scisoft/setup         

setup git    v2_20_1     -q e17                                                 
setup cmake  v3_14_3                                                            
setup boost  v1_75_0     -q e19:prof                                            
setup root   v6_18_04    -q e19:prof                                            
setup geant4 v4_10_6_p01 -q e19:prof                                            

cd /n/holystore01/LABS/guenette_lab/Users/jh/software/marley/v1.2.0/build && source ../setup_marley.sh && cd ${OLDPWD}
