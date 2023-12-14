// -----------------------------------------------------------------------------
//  G4_QPIX | NeutronBackground.H
//
//  Redefining spectrum of photons from neutron capture and distributing them 
//  into the detector.
//   * Author: Gabriela Vitti Stenico
//   * Creation date: 08 Dec 2023
//
//  GVS: 08 Dec 2023
//       ATTENTION! USE THE FOLLOWING COMMAND LINES WHEN 
//       RUNNING QPIXG4 FOR NEUTRON BACKGROUND STUDIES:
//       1) ./app/G4_QPIX ../macros/neutron_surf.mac | awk '/NCapGamma:/ {print $4,$5,$6,$7,$8,$9,$10}' > ../output/photon_ncap.txt 
//       -> Gets photons from neutron capture and 
//          saves their position, multiplicity and initial 
//          momentum in '../output/photon_ncap.txt'
//          IF YOU WANT TO CHANGE THE TEXT FILE NAME, BE SURE 
//          TO ALSO MODIFY THE FILE NAME IN 'NeutronBackground.cc'
//       2) ./app/G4_QPIX ../macros/Template_Neutron_Background.mac | awk '/NCapElec:/ {print $2,$3,$4,$5,$6,$7}' > ../output/electron_ncap.txt
//       -> Generates secondary electrons from these photons.
//          ( | awk '/NCapElec:/ {print $2,$3,$4,$5,$6,$7}' > ../output/electron_ncap.txt ) is optional.
//          Just be sure you have '/neutron/get_electrons 1' in your macro.
// -----------------------------------------------------------------------------

#include "NeutronBackground.h"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PhysicalConstants.hh"

#include "MCTruthManager.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <random>
#include <vector>


NeutronBackground::NeutronBackground()
: G4VPrimaryGenerator()
{ 
  myfile.open("../output/photon_ncap.txt");
}

NeutronBackground::~NeutronBackground()
{ 
  myfile.close();
}

void NeutronBackground::GeneratePrimaryVertex(G4Event *anEvent)
{

  G4int line=0;
  G4double ener,x,y,z,cosx,cosy,cosz,time;
  
  std::vector <std::vector <double>> GammaDist;
 
  while ( myfile ) {
     myfile >> x >> y >> z >> cosx >> cosy >> cosz >> time; 
     GammaDist.push_back({x,y,z,cosx,cosy,cosz,time});
     line++;
  }

   GammaDist.resize(line);
  
  // Uncomment it if you want to print what program is reading from 'myfile' on terminal screen
  /*for(G4int j=0;j<GammaDist.size(); j++){
           G4cout << GammaDist[j][0] << " " 
                  << GammaDist[j][1] << " " 
                  << GammaDist[j][2] << " " 
                  << GammaDist[j][3] << " " 
                  << GammaDist[j][4] << " " 
                  << GammaDist[j][5] << " " 
                  << GammaDist[j][6] << " "
                  << line<< G4endl;
  }*/

  // get MC truth manager
  MCTruthManager * mc_truth_manager = MCTruthManager::Instance();

  // Photon Spectrum from Neutron Capture of the Nuclear Data Center
  // See: https://www.nndc.bnl.gov/capgam/byTarget/41AR_40AR-N-G-POL-N-G-E-THERMAL.html
  
  std::random_device rd;
  std::mt19937 gen(rd());

  std::vector<double> en {0.1671, 0.1675, 0.3484, 0.349, 0.5157, 0.5163, 0.8374, 0.838, 
                          0.8667, 0.8679, 1.0439, 1.0447, 1.1865, 1.1871, 1.3536, 1.3544, 
                          1.8276, 1.83, 1.8805, 1.8825, 1.9714, 1.9738, 2.1299, 2.1315, 2.2275, 
                          2.2315, 2.2896, 2.2936, 2.4317, 2.4333, 2.5653, 2.5669, 2.6135, 
                          2.6151, 2.6661, 2.6701, 2.6661, 2.6701, 2.771, 2.7726, 2.7803, 
                          2.7833, 2.8097, 2.8113, 2.8415, 2.8435, 3.0884, 3.0904, 3.1091, 
                          3.1135, 3.1492, 3.1512, 3.3645, 3.3665, 3.4028, 3.4078, 3.403, 3.408, 
                          3.4508, 3.4528, 3.562, 3.567, 3.5622, 3.5672, 3.6567, 3.6603, 3.6996, 
                          3.7012, 4.101, 4.104, 4.7442, 4.7458, 4.9148, 4.9188, 5.0627, 5.0647, 
                          5.4461, 5.4511, 5.5812, 5.5828, 5.9577, 5.9627, 6.0601, 6.0651, 
                          6.0798, 6.0848, 6.0903, 6.0953, 6.1395, 6.1445};
  std::vector<double> w  {100., 0, 8.2915, 0, 31.7839, 0, 12.0603, 0, 1.3819, 0, 7.5377, 0, 
                          65.5779, 0, 2.8894, 0, 1.2563, 0, 1.7588, 0, 0.6784, 0, 5.5276, 0, 
                          0.3643, 0, 0.2513, 0, 1.0553, 0, 3.5176, 0, 3.6432, 0, 0.6407, 0, 
                          0.6407, 0, 10.804, 0, 2.1357, 0, 7.4121, 0, 1.1055, 0, 1.3819, 0, 
                          0.5025, 0, 5.0251, 0, 5.2764, 0, 0.1005, 0, 0.1005, 0, 2.5126, 0, 
                          0.1633, 0, 0.1633, 0, 0.3141, 0, 12.3116, 0, 0.3769, 0, 69.0955, 0, 
                          0.0879, 0, 0.3266, 0, 0.0628, 0, 14.5729, 0, 0.0126, 0, 0.0503, 0, 
                          0.0251, 0, 0.0377, 0, 0.0251};
  
  std::piecewise_constant_distribution<> d(en.begin(), en.end(), w.begin());
 
  std::map<int, int> hist;
    
  /* Uncomment it if you want to print the photon energies on terminal screen
  for (G4int n =0; n < line; ++n){
        ++hist[d(gen)];
        G4cout << d(gen) << G4endl;
  }*/
 

  for(G4int k=0;k<GammaDist.size(); k++){

    G4PrimaryVertex* vertexA = new G4PrimaryVertex({GammaDist[k][0],GammaDist[k][1],GammaDist[k][2]}, GammaDist[k][6]);
  
    G4ParticleDefinition* particleDefinition
           = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    G4PrimaryParticle* particle1 = new G4PrimaryParticle(particleDefinition);
    particle1->SetMomentumDirection({GammaDist[k][3],GammaDist[k][4],GammaDist[k][5]});    
    ener = d(gen);
    particle1->SetKineticEnergy(ener);
  
    vertexA->SetPrimary(particle1);
    anEvent->AddPrimaryVertex(vertexA);


    // create generator particle
    GeneratorParticle * particleNCap = new GeneratorParticle();
    particleNCap->SetPDGCode (  particle1->GetPDGcode()     );  
    particleNCap->SetMass    (  particle1->GetMass()        );  
    particleNCap->SetCharge  (  particle1->GetCharge()      ); 
    particleNCap->SetX       (  GammaDist[k][0]/CLHEP::cm   ); 
    particleNCap->SetY       (  GammaDist[k][1]/CLHEP::cm   );
    particleNCap->SetZ       (  GammaDist[k][2]/CLHEP::cm   );
    particleNCap->SetT       (  GammaDist[k][6] /*in ns*/   );
    particleNCap->SetPx      (  particle1->GetMomentum().x());
    particleNCap->SetPy      (  particle1->GetMomentum().y());
    particleNCap->SetPz      (  particle1->GetMomentum().z());
    particleNCap->SetEnergy  (  ener                        );

    // add initial photons from Neutron Capture
    mc_truth_manager->AddInitialGeneratorParticle(particleNCap);
    mc_truth_manager->AddFinalGeneratorParticle(particleNCap);
  }
  
}