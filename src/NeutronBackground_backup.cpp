// -----------------------------------------------------------------------------
//  G4_QPIX | NeutronBackground.cpp
//
//  Redefining spectrum of photons from neutron capture and distributing them 
//  into the detector.
//   * Author: Gabriela Vitti Stenico
//   * Creation date: 08 Dec 2023
//   * Updated on 23 April 2024
//
//  GVS: 23 April 2024
//       ATTENTION! USE THE FOLLOWING COMMAND LINES WHEN 
//       RUNNING QPIXG4 FOR NEUTRON BACKGROUND STUDIES:
//       cd PathToQPIXG4directory/qpixg4
//       source run_neutron_background.sh
// -----------------------------------------------------------------------------

#include "NeutronBackground.h"
#include "ConfigManager.h"
#include "MCTruthManager.h"

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

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <random>
#include <vector>


NeutronBackground::NeutronBackground()
{ 
  evfile = "../output/photon_ncap.txt";
  inputfile.open((char*)evfile);
  
  if (inputfile.is_open()){
    G4cout << "photon_ncap.txt file is open." << G4endl;
  }
 /* else{
    G4Exception("NeutronBackground::NeutronBackground","NB01", FatalException, "Cannot open file photon_ncap.txt.");
  }*/
}


void NeutronBackground::GeneratePrimaryVertex(G4Event *anEvent)
{
  // get MC truth manager
  MCTruthManager * mc_truth_manager = MCTruthManager::Instance();
  NBverbose = ConfigManager::GetNBVerbose();

  G4int Nentries = 0;  // number of entries
  G4int Evt=0;
  G4double x,y,z,time;

  if (inputfile.is_open()){
    inputfile >> Nentries >> Evt >> x >> y >> z >> time;
  }
  else{
    G4Exception("NeutronBackground::GeneratePrimaryVertex","NB02", FatalException, "Cannot open file photon_ncap.txt.");
  }  

  if(Nentries == 0){
    G4Exception("NeutronBackground::GeneratePrimaryVertex", "NB03",RunMustBeAborted, "No primary photons.");
    return;
  }

  if(NBverbose > 0){
  G4cout << "NeutronBackground - reading event number " << Evt << G4endl;
  G4cout << "NeutronBackground - reading " << Nentries << " photons from file." << G4endl;
  }

  G4PrimaryVertex* vertexA = new G4PrimaryVertex({x,y,z}, time);
  G4double sumEn =0.0;

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

  std::vector<double> vertEn(Nentries, 0.0);

  for( G4int k=0; k<Nentries; k++ ){
    vertEn[k] = d(gen);
    
    if(sumEn + vertEn[k] < 6.2){sumEn = sumEn + vertEn[k];}
    else { vertEn[k] = 0.0;}
    
    G4cout << "Energia da partícula é: " << vertEn[k] << G4endl;
    G4cout << "Energia total do vertice é: " << sumEn << G4endl;
  }

  for( G4int i=0; i<Nentries; i++ ){
  
    G4double cosx,cosy,cosz;

    inputfile >> cosx >> cosy >> cosz; 

    if( inputfile.eof() ) {
      G4Exception("NeutronBackground::GeneratePrimaryVertex", "NB04", FatalException, "unexpected end-of-file in the middle of an event");
    }
 
    if(NBverbose > 1){
    G4cout << x << " "
           << y << " "
           << z << " "
           << cosx << " "
           << cosy << " " 
           << cosz << " "
           << time << " New energy is: " 
           << vertEn[i] << G4endl;
    }
  
  if(vertEn[i] != 0.0){
    // Create Primary Photons
    G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    G4PrimaryParticle* particle1 = new G4PrimaryParticle(particleDefinition);
    particle1->SetMomentumDirection({cosx,cosy,cosz});    
  //  ener = d(gen);
    particle1->SetKineticEnergy(vertEn[i] /* ener */);
    G4cout << "SinEnergy: " << vertEn[i] << G4endl;
    vertexA->SetPrimary(particle1);

    // create generator particle
    GeneratorParticle * particleNCap = new GeneratorParticle();
    particleNCap->SetPDGCode (  particle1->GetPDGcode()     );  
    particleNCap->SetMass    (  particle1->GetMass()        );  
    particleNCap->SetCharge  (  particle1->GetCharge()      ); 
    particleNCap->SetX       (  x/CLHEP::cm   ); 
    particleNCap->SetY       (  y/CLHEP::cm   );
    particleNCap->SetZ       (  z/CLHEP::cm   );
    particleNCap->SetT       (  time /*in ns*/   );
    particleNCap->SetPx      (  particle1->GetMomentum().x());
    particleNCap->SetPy      (  particle1->GetMomentum().y());
    particleNCap->SetPz      (  particle1->GetMomentum().z());
    particleNCap->SetEnergy  (  vertEn[i]  /* ener */       );

    // add initial photons from Neutron Capture
    mc_truth_manager->AddInitialGeneratorParticle(particleNCap);
    }
  }
 
  if(sumEn>0) {G4cout << "TotEnergy: " << sumEn << G4endl;}  

  if(vertexA->GetNumberOfParticle() == 0) {
    G4Exception("NeutronBackground::GeneratePrimaryVertex", "NB05",RunMustBeAborted, "Vertex with no particles! Probabaly you reached the end-of-file.");
    return;
  }
  anEvent->SetEventID(Evt);
  anEvent->AddPrimaryVertex(vertexA);
}