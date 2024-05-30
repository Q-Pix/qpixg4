cd build
./app/G4_QPIX ../macros/neutron_surf.mac | awk '/NCapGamma:/ {print $4,$5,$6,$7,$8,$9 }' > ../output/photon_ncap.txt

python3 ../macros/WritePhotons.py
./app/G4_QPIX ../macros/Template_Neutron_Background_forRun.mac | awk '/NCapElec:/ {print $4,$5,$6,$7,$8,$9}' > ../output/electron_ncap.txt

cd ..
