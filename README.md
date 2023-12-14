## Please see the Wiki for details!
https://github.com/Q-Pix/Q_PIX_GEANT4/wiki

##  GVS: 08 Dec 2023
##       ATTENTION! USE THE FOLLOWING COMMAND LINES WHEN 
##       RUNNING QPIXG4 FOR NEUTRON BACKGROUND STUDIES:
##       1) ./app/G4_QPIX ../macros/neutron_surf.mac | awk '/NCapGamma:/ {print $4,$5,$6,$7,$8,$9,$10}' > ../output/photon_ncap.txt 
##       -> Gets photons from neutron capture and 
##          saves their position, multiplicity and initial 
##          momentum in '../output/photon_ncap.txt'
##          IF YOU WANT TO CHANGE THE TEXT FILE NAME, BE SURE 
##          TO ALSO MODIFY THE FILE NAME IN 'NeutronBackground.cc'
##       2) ./app/G4_QPIX ../macros/Template_Neutron_Background.mac | awk '/NCapElec:/ {print $2,$3,$4,$5,$6,$7}' > ../output/electron_ncap.txt
##       -> Generates secondary electrons from these photons.
##          ( | awk '/NCapElec:/ {print $2,$3,$4,$5,$6,$7}' > ../output/electron_ncap.txt ) is optional.
##          Just be sure you have '/neutron/get_electrons 1' in your macro.
## -----------------------------------------------------------------------------
