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

  G4int Evt=0;
  G4int isotope;
  G4double x,y,z,time;

  if (inputfile.is_open()){
    inputfile >> isotope >> Evt >> x >> y >> z >> time;
  }
  else{
    G4Exception("NeutronBackground::GeneratePrimaryVertex","NB02", FatalException, "Cannot open file photon_ncap.txt.");
  }

  if(NBverbose > 0){
  G4cout << "NeutronBackground - reading event number " << Evt << G4endl;
  }

  G4PrimaryVertex* vertexA = new G4PrimaryVertex({x,y,z}, time);

  std::vector<double> energies;

  if(isotope == 41){
  
  G4double PrimEner = 0.0;
  G4double SecEner0516 = 0.0;
  G4double SecEner3968 = 0.0;
  G4double SecEner1353 = 0.0;
  G4double SecEner3326 = 0.0;
  G4double SecEner2948 = 0.0;
  G4double SecEner2398 = 0.0;

  // Nuclear Transitions of the gammas in Argon-41
  // See: https://www.nndc.bnl.gov/ensnds/41/Ar/ng_pol_n_g_E_thermal.pdf, Page 3


  std::random_device rd;
  std::mt19937 gen(rd()); 
  
  /**************************** Transitions *******************************************************/

  // Energy shell: 6099 keV
   std::vector<double> en6099 {1.8282, 1.8294, 2.1303, 2.1311, 2.7714, 2.7722,
                              3.0889, 3.0899, 3.1497, 3.1507, 3.365, 3.366, 3.7, 
                              3.7008, 4.7446, 4.7454, 5.0632, 5.0642, 5.5816, 5.5824};
   std::vector<double> w6099 {1.00041, 0., 4.40173, 0., 8.60342, 0., 1.10043, 0., 4.00158, 0.,
                              4.20169, 0., 9.80395, 0., 55.022, 0., 0.260078, 0., 11.6047}; 

/*   std::vector<double> en6099 {1.8282, 1.8294, 2.1303, 2.1311, 2.6671, 2.6691, 2.7714, 2.7722,
                              3.0889, 3.0899, 3.1497, 3.1507, 3.365, 3.366, 3.40405, 3.40655, 3.7, 
                              3.7008, 4.7446, 4.7454, 5.0632, 5.0642, 5.5816, 5.5824};
  std::vector<double> w6099 {1.2563, 0., 5.5276, 0., 0.6407, 0., 10.804, 0., 1.3819, 0., 5.0251, 
                            0., 5.2764, 0., 0.1005, 0., 12.3116, 0., 69.0955, 0., 0.3266, 0., 14.5729}; */
  std::piecewise_constant_distribution<> d6099(en6099.begin(), en6099.end(), w6099.begin());

  // Energy shell: 4270.0 keV
  std::vector<double> en4270 {4.10175, 4.10325};
  std::vector<double> w4270 {100.0};
  std::piecewise_constant_distribution<> d4270(en4270.begin(), en4270.end(), w4270.begin());

  // Energy shell: 3968.2 keV 
  std::vector<double> en3968 {2.6139, 2.6147, 3.4513, 3.4523};
  std::vector<double> w3968 {59.0, 0.0, 41.0};
  std::piecewise_constant_distribution<> d3968(en3968.begin(), en3968.end(), w3968.begin());

  // Energy shell: 3326.8 keV
  std::vector<double> en3326 {1.972, 1.9732, 2.2906, 2.2926, 2.8101, 2.8109};
  std::vector<double> w3326 {8.0, 0.0, 3.0, 0.0, 89.0};
  std::piecewise_constant_distribution<> d3326(en3326.begin(), en3326.end(), w3326.begin());

  // Energy shell: 3009.6 keV
  std::vector<double> en3009 {2.842, 2.843};
  std::vector<double> w3009 {100.0};
  std::piecewise_constant_distribution<> d3009(en3009.begin(), en3009.end(), w3009.begin());

  // Energy shell: 2948.7 keV
  std::vector<double> en2948 {2.4321, 2.4329, 2.78105, 2.78255};
  std::vector<double> w2948 {32.0, 0.0, 68.0};
  std::piecewise_constant_distribution<> d2948(en2948.begin(), en2948.end(), w2948.begin());

  // Energy shell: 2733.4 keV
  std::vector<double> en2733 {2.5657, 2.5665};
  std::vector<double> w2733 {100.0};
  std::piecewise_constant_distribution<> d2733(en2733.begin(), en2733.end(), w2733.begin());

  // Energy shell: 2398.1 keV
  std::vector<double> en2398 {1.0441, 1.0445, 1.881, 1.882, 2.2285, 2.2305};
  std::vector<double> w2398 {78.0, 0.0, 18.0, 0.0, 4.0};
  std::piecewise_constant_distribution<> d2398(en2398.begin(), en2398.end(), w2398.begin());

  // Energy shell: 1353.9 keV
  std::vector<double> en1353 {0.83755, 0.83785, 1.18665, 1.18695, 1.3538, 1.3542};
  std::vector<double> w1353 {15.0, 0.0, 81.0, 0.0, 4.0};
  std::piecewise_constant_distribution<> d1353(en1353.begin(), en1353.end(), w1353.begin());

  // Energy shell: 1034.7 keV
  std::vector<double> en1034 {0.867, 0.8676};
  std::vector<double> w1034 {100.0};
  std::piecewise_constant_distribution<> d1034(en1034.begin(), en1034.end(), w1034.begin());

  // Energy shell: 516.1 keV
  std::vector<double> en0516 {0.34855, 0.34885, 0.51585, 0.51615};
  std::vector<double> w0516 {20.0, 0.0, 80.0};
  std::piecewise_constant_distribution<> d0516(en0516.begin(), en0516.end(), w0516.begin());

  // Energy shell: 167.3 keV
  std::vector<double> en0167 {0.1672, 0.1674};
  std::vector<double> w0167 {100.0};
  std::piecewise_constant_distribution<> d0167(en0167.begin(), en0167.end(), w0167.begin());
  
  // First step: generate the energy transition for the gammas at the most energetic nuclear shell
  PrimEner = d6099(gen);

  energies.push_back(PrimEner);

  /*************************************************************************************/
  // Second step: generate the transition of the gammas after the first deexcitation.
  // We will have 10 cases here
  /*************************************************************************************/
  
  // Energy transition = 1828.8 keV (from shell 6099 keV to shell 4270.0 keV)
  // Is followed by a energy transition of 4102.5 keV (to shell 167.3 keV)
  // Then, by a energy transition of 167.3 keV (to ground)
  if(1.8282 <= PrimEner && PrimEner <= 1.8294){

      // 4102.5 keV (to shell 4270.0 keV)
      energies.push_back(d4270(gen));

      // 167.3 keV (to ground)
      energies.push_back(d0167(gen));
  }

  // Energy transition = 2130.7 keV (from shell 6099 keV to shell 3968.2 keV)
  // Followed by 3451.8 keV (to shell 516.1 keV) or 2614.3 keV (to shell 1353.9 keV) transitions
  if(2.1303 <= PrimEner && PrimEner <= 2.1311){

      // 3451.8 keV (to shell 516.1 keV) or 2614.3 keV (to shell 1353.9 keV)
      SecEner3968 = d3968(gen);

      energies.push_back(SecEner3968);

      // Energy transition = 2614.3 keV
      // Followed by 1354.0 keV (to ground), 1186.8 keV (shell 167.3 keV) or 837.7 keV (to shell 516.1 keV)
      if(2.6139 <= SecEner3968 && SecEner3968 <= 2.6147){

        // 1354.0 keV (to ground), 1186.8 keV (to shell 167.3 keV) or 837.7 keV (to shell 516.1 keV)
        SecEner1353 = d1353(gen);

        energies.push_back(SecEner1353);

        // Energy transition = 837.7 keV (to shell 516.0 keV)
        if(0.83755 <= SecEner1353 && SecEner1353 <= 0.83785){

          // 348.7 keV (to shell 167.3 keV) or 516.0 keV (to ground)
          SecEner0516 = d0516(gen);

          energies.push_back(SecEner0516);

          // Energy transition = 348.7 keV
          // Followed by 167.3 keV (to ground)
          if(0.34855 <= SecEner0516 && SecEner0516 <= 0.34885){
            energies.push_back(d0167(gen));
          }
        }
     
        // Energy transition = 1186.8 keV (to shell 167.3 keV)
        // Followed by 167.3 keV (to ground)
        if(1.18665 <= SecEner1353 && SecEner1353 <= 1.18695){
            energies.push_back(d0167(gen));
        }  

      }

      // Energy transition = 3451.8 keV
      // Followed by 516.0 keV or 348.7 keV
      if(3.4513 <= SecEner3968 && SecEner3968 <= 3.4523){

        // 348.7 keV or 516.0 keV
        SecEner0516 = d0516(gen);

        energies.push_back(SecEner0516);

        // Energy transition = 348.7 keV
        // Followed by 167.3 keV
        if(0.34855 <= SecEner0516 && SecEner0516 <= 0.34885){
          energies.push_back(d0167(gen));
        }

      }
  }

  // Energy transition = 2771.8 keV (from shell 6099 keV to shell 3326.8 keV)
  // Followed by 2810.5 keV (to shell 516.1 keV), 2291.6 keV (shell 1034.7 keV) or 1972.6 keV (to shell 1353.9 keV)
  if(2.7714 <= PrimEner && PrimEner <= 2.7722){

    // 2810.5 keV (to shell 516.1 keV), 2291.6 keV (to shell 1034.7 keV) or 1972.6 keV (to shell 1353.9 keV)
    SecEner3326 = d3326(gen);

    energies.push_back(SecEner3326);
 
    // Energy transition = 2810.5 keV (to shell 516.1 keV)
    if(2.8101 <= SecEner3326 && SecEner3326 <= 2.8109){
        
        // 348.7 keV or 516.0 keV
        SecEner0516 = d0516(gen);

        energies.push_back(SecEner0516);

        // Energy transition = 348.7 keV
        // Followed by 167.3 keV
        if(0.34855 <= SecEner0516 && SecEner0516 <= 0.34885){
          energies.push_back(d0167(gen));
        }
    }

    // Energy transition = 2291.6 keV (to shell 1034.7 keV)
    if(2.2906 <= SecEner3326 && SecEner3326 <= 2.2926){
        
        // 867.3 keV (to shell 167.3 keV)
        energies.push_back(d1034(gen));

        // 167.3 keV (to ground)
        energies.push_back(d0167(gen));
    }

    // Energy transition = 1972.6 keV (to shell 1353.9 keV)
    if(1.972 <= SecEner3326 && SecEner3326 <= 1.9732){
        
        // 1354.0 keV (to ground), 1186.8 keV (to shell 167.3 keV) or 837.7 keV (to shell 516.1 keV)
        SecEner1353 = d1353(gen);

        energies.push_back(SecEner1353);

        // Energy transition = 837.7 keV (to shell 516.0 keV)
        if(0.83755 <= SecEner1353 && SecEner1353 <= 0.83785){

          // 348.7 keV (to shell 167.3 keV) or 516.0 keV (to ground)
          SecEner0516 = d0516(gen);

          energies.push_back(SecEner0516);

          // Energy transition = 348.7 keV
          // Followed by 167.3 keV (to ground)
          if(0.34855 <= SecEner0516 && SecEner0516 <= 0.34885){
            energies.push_back(d0167(gen));
          }
        }
     
        // Energy transition = 1186.8 keV (to shell 167.3 keV)
        // Followed by 167.3 keV (to ground)
        if(1.18665 <= SecEner1353 && SecEner1353 <= 1.18695){
            energies.push_back(d0167(gen));
        }  
    }
  }

  // Energy transition = 3089.4 keV (from shell 6099 keV to shell 3009.6 keV)
  // Is followed by a energy transition of 2842.5 keV (to shell 167.3 keV)
  // Then, by a energy transition of 167.3 keV (to ground)
  if(3.0889 <= PrimEner && PrimEner <=  3.0899){

    // 2842.5 keV (to shell 167.3 keV)
    energies.push_back(d3009(gen));

    // 167.3 keV (to ground)
    energies.push_back(d0167(gen));

  }

  // Energy transition = 3150.2 keV (from shell 6099 keV to shell 2948.7 keV)
  // Is followed by a energy transition of 2781.8 keV (to shell 167.3 keV) or 2432.5 keV (to shell 516.1 keV)
  if(3.1497 <= PrimEner && PrimEner <= 3.1507){

    // 2781.8 keV (to shell 167.3 keV) or 2432.5 keV (to shell 516.1 keV)
    SecEner2948 = d2948(gen);

    energies.push_back(SecEner2948);

    // Energy transition = 2810.5 keV (to shell 167.3 keV)
    if(2.78105 <= SecEner2948 && SecEner2948 <= 2.78255){

        // 167.3 keV (to shell ground) 
        energies.push_back(d0167(gen));
    }

    // Energy transition = 2432.5 keV (to shell 516.0 keV)
    if(2.4321 <= SecEner2948 && SecEner2948 <= 2.4329){

        // 348.7 keV (to shell 167.3 keV) or 516.0 keV (to ground)
        SecEner0516 = d0516(gen);

        energies.push_back(SecEner0516);

        // Energy transition = 348.7 keV
        // Followed by 167.3 keV (to ground)
        if(0.34855 <= SecEner0516 && SecEner0516 <= 0.34885){
            energies.push_back(d0167(gen));
        }
    }

  }

  // Energy transition = 3365.5 keV (from shell 6099 keV to shell 2733.4 keV)
  // Is followed by a energy transition of 2566.1 keV (to shell 167.3 keV)
  if(3.365 <= PrimEner && PrimEner <= 3.366){

    // 2842.5 keV (to shell 167.3 keV)
    energies.push_back(d2733(gen));

    // 167.3 keV (to ground)
    energies.push_back(d0167(gen));

  }

// Energy transition = 3700.4 keV (from shell 6099 keV to shell 2398.1 keV)
// Followed by 2229.5 keV (to shell 167.3 keV), 1881.5 keV (shell 516.1 keV) or 1044.3 keV (to shell 1353.9 keV)
  if(3.7 <= PrimEner && PrimEner <= 3.7008){

    // 2229.5 keV (to shell 167.3 keV), 1881.5 keV (shell 516.1 keV) or 1044.3 keV (to shell 1353.9 keV)
    SecEner2398 = d2398(gen);

    energies.push_back(SecEner2398);
 
    // Energy transition = 1044.3 keV (to shell 1353.9 keV)
    if(1.0441 <= SecEner2398 && SecEner2398 <= 1.0445){

      // 1354.0 keV (to ground), 1186.8 keV (to shell 167.3 keV) or 837.7 keV (to shell 516.1 keV)
      SecEner1353 = d1353(gen);

      energies.push_back(SecEner1353);

      // Energy transition = 837.7 keV (to shell 516.0 keV)
      if(0.83755 <= SecEner1353 && SecEner1353 <= 0.83785){

        // 348.7 keV (to shell 167.3 keV) or 516.0 keV (to ground)
        SecEner0516 = d0516(gen);

        energies.push_back(SecEner0516);

        // Energy transition = 348.7 keV
        // Followed by 167.3 keV (to ground)
        if(0.34855 <= SecEner0516 && SecEner0516 <= 0.34885){
           energies.push_back(d0167(gen));
        }
      }
     
      // Energy transition = 1186.8 keV (to shell 167.3 keV)
      // Followed by 167.3 keV (to ground)
      if(1.18665 <= SecEner1353 && SecEner1353 <= 1.18695){
          energies.push_back(d0167(gen));
      }  
    }
    
    // Energy transition = 1881.5 keV (shell 516.1 keV)
    if(1.881 <= SecEner2398 && SecEner2398 <= 1.882){
    
      // 348.7 keV or 516.0 keV
      SecEner0516 = d0516(gen);

      energies.push_back(SecEner0516);

      // Energy transition = 348.7 keV
      // Followed by 167.3 keV
      if(0.34855 <= SecEner0516 && SecEner0516 <= 0.34885){
          energies.push_back(d0167(gen));
      }

    }

    // Energy transition = 2229.5 keV (to shell 167.3 keV)
    if(2.2285 <= SecEner2398 && SecEner2398 <= 2.2305){
      
      // 167.3 keV (to ground)
      energies.push_back(d0167(gen));
    
    }
  }

  // Energy transition = 4745.0 keV (from shell 6099 keV to shell 1353.9 keV)
  // Followed by 1354.0 keV (to ground), 1186.8 keV (to shell 167.3 keV) or 837.7 keV (to shell 516.1 keV)
  if(4.7446 <= PrimEner && PrimEner <= 4.7454){

    // 1354.0 keV (to ground), 1186.8 keV (to shell 167.3 keV) or 837.7 keV (to shell 516.1 keV)
    SecEner1353 = d1353(gen); 
    
    energies.push_back(SecEner1353);

    // Energy transition = 837.7 keV (to shell 516.0 keV)
    if(0.83755 <= SecEner1353 && SecEner1353 <= 0.83785){

      // 348.7 keV (to shell 167.3 keV) or 516.0 keV (to ground)
      SecEner0516 = d0516(gen);

      energies.push_back(SecEner0516);

      // Energy transition = 348.7 keV
      // Followed by 167.3 keV (to ground)
      if(0.34855 <= SecEner0516 && SecEner0516 <= 0.34885){
           energies.push_back(d0167(gen));
        }
      }
     
      // Energy transition = 1186.8 keV (to shell 167.3 keV)
      // Followed by 167.3 keV (to ground)
      if(1.18665 <= SecEner1353 && SecEner1353 <= 1.18695){
          energies.push_back(d0167(gen));
      }  

  }

  // Energy transition = 5063.7 keV (from shell 6099 keV to shell 1034.7 keV)
  // Followed by 867.3 keV (to shell 167.3 keV) and a 167.3 keV transition (to ground)
  if(5.0632 <= PrimEner && PrimEner <= 5.0642){

        // 867.3 keV (to shell 167.3 keV)
        energies.push_back(d1034(gen));

        // 167.3 keV (to ground)
        energies.push_back(d0167(gen));

  }

  // Energy transition = 5582.0 keV (from shell 6099 keV to shell 516.1 keV)
  // Followed by 348.7 keV (to shell 167.3 keV) and a 167.3 keV transition (to ground)
  if(5.5816 <= PrimEner && PrimEner <= 5.5824){

    // 348.7 keV or 516.0 keV
    SecEner0516 = d0516(gen);

    energies.push_back(SecEner0516);

    // Energy transition = 348.7 keV
    // Followed by 167.3 keV
    if(0.34855 <= SecEner0516 && SecEner0516 <= 0.34885){
        energies.push_back(d0167(gen));
    }
  }
  }

  if(isotope==37){

  G4double PrimEner = 0.0;
  G4double SecEner2490 = 0.0;
  G4double SecEner5090 = 0.0;
  G4double SecEner4637 = 0.0;
  G4double SecEner3518 = 0.0;

  // Nuclear Transitions of the gammas in Argon-37
  // See: https://www.nndc.bnl.gov/ensnds/37/Ar/ng_E_thermal.pdf, Page 3


  std::random_device rd;
  std::mt19937 gen(rd()); 
  
  /**************************** Transitions *******************************************************/

  // Energy shell: 8791.2 keV
  std::vector<double> en8791 {1.9652, 1.9682, 2.2066, 2.2086, 3.7, 3.7004, 4.1525, 4.1535, 4.2111,
                                4.2121, 4.34205, 4.34255, 4.81005, 4.81055, 4.85105, 4.85255,
                                5.27215, 5.27305, 6.2994, 6.3, 8.79, 8.7908};
  std::vector<double> w8791 {4.2105, 0., 4.7368, 0., 23.5088, 0., 3.1579, 0., 1.4035, 0., 5.614, 
                              0., 2.807, 0., 1.4035, 0., 43.8596, 0., 65.7895, 0., 19.1228}; 
  std::piecewise_constant_distribution<> d8791(en8791.begin(), en8791.end(), w8791.begin());

  // Energy shell: 6826.2 keV
  std::vector<double> en6826 {2.24715, 2.24865};
  std::vector<double> w6826 {100.0};
  std::piecewise_constant_distribution<> d6826(en6826.begin(), en6826.end(), w6826.begin());

  // Energy shell: 6583.7 keV 
  std::vector<double> en6583 {2.1343, 2.1363};
  std::vector<double> w6583 {100.0};
  std::piecewise_constant_distribution<> d6583(en6583.begin(), en6583.end(), w6583.begin());

  // Energy shell: 5090.5 keV
  std::vector<double> en5090 {2.5994, 2.5998, 3.6791, 3.6795};
  std::vector<double> w5090 {28.18, 0.0, 71.82};
  std::piecewise_constant_distribution<> d5090(en5090.begin(), en5090.end(), w5090.begin());

  // Energy shell: 4637.6 keV
  std::vector<double> en4637 {2.1442, 2.1462, 3.2259, 3.2279};
  std::vector<double> w4637 {16.6667, 0.0, 83.3333};
  std::piecewise_constant_distribution<> d4637(en4637.begin(), en4637.end(), w4637.begin());

  // Energy shell: 4578.7 keV
  std::vector<double> en4578 {2.0868, 2.0878};
  std::vector<double> w4578 {100.0};
  std::piecewise_constant_distribution<> d4578(en4578.begin(), en4578.end(), w4578.begin());

  // Energy shell: 4448.6 keV
  std::vector<double> en4448 {1.9563, 1.9583};
  std::vector<double> w4448 {100.0};
  std::piecewise_constant_distribution<> d4448(en4448.begin(), en4448.end(), w4448.begin());

  // Energy shell: 3981.1 keV
  std::vector<double> en3981 {3.98115, 3.98165};
  std::vector<double> w3981 {100.0};
  std::piecewise_constant_distribution<> d3981(en3981.begin(), en3981.end(), w3981.begin());

  // Energy shell: 3938.5 keV
  std::vector<double> en3938 {3.9375, 3.9385};
  std::vector<double> w3938 {100.0};
  std::piecewise_constant_distribution<> d3938(en3938.begin(), en3938.end(), w3938.begin());

  // Energy shell: 3518.0 keV
  std::vector<double> en3518 {1.02645, 1.02695, 2.1073, 2.1077};
  std::vector<double> w3518 {10.22, 0.0, 89.78};
  std::piecewise_constant_distribution<> d3518(en3518.begin(), en3518.end(), w3518.begin());

  // Energy shell: 2490.9 keV
  std::vector<double> en2490 {0.878, 0.879, 2.4904, 2.4908};
  std::vector<double> w2490 {0.87, 0.0, 99.13};
  std::piecewise_constant_distribution<> d2490(en2490.begin(), en2490.end(), w2490.begin());

  // Energy shell: 1611.9 keV
  std::vector<double> en1611 {1.61135, 1.61205};
  std::vector<double> w1611 {100.0};
  std::piecewise_constant_distribution<> d1611(en1611.begin(), en1611.end(), w1611.begin());

  // Energy shell: 1410.6 keV
  std::vector<double> en1410 {1.41, 1.4106};
  std::vector<double> w1410 {100.0};
  std::piecewise_constant_distribution<> d1410(en1410.begin(), en1410.end(), w1410.begin());


  PrimEner = d8791(gen);

  energies.push_back(PrimEner);

  if(1.9652 <= PrimEner && PrimEner <= 1.9682){
     
      energies.push_back(d6826(gen));

      energies.push_back(d4578(gen));

      SecEner2490 = d2490(gen);

      energies.push_back(SecEner2490);

      if(0.878 <= SecEner2490 && SecEner2490 <= 0.879){
        energies.push_back(d1611(gen));
      }
  }

  if(2.2066 <= PrimEner && PrimEner <= 2.2086){

    energies.push_back(d6583(gen));

    energies.push_back(d4448(gen));

    SecEner2490 = d2490(gen);

    energies.push_back(SecEner2490);

    if(0.878 <= SecEner2490 && SecEner2490 <= 0.879){
      energies.push_back(d1611(gen));
    }
  }

  if(3.7 <= PrimEner && PrimEner <= 3.7004){
    
    SecEner5090 = d5090(gen);

    energies.push_back(SecEner5090);

    if(2.5994<= SecEner5090 && SecEner5090 <= 2.5998){

      SecEner2490 = d2490(gen);
      
      energies.push_back(SecEner2490);
      
      if(0.878 <= SecEner2490 && SecEner2490 <= 0.879){
        energies.push_back(d1611(gen));
      }
    }

    if(3.6791<= SecEner5090 && SecEner5090 <= 3.6795){

      energies.push_back(d1410(gen));

    }

  }

  if(4.1525 <= PrimEner && PrimEner <= 4.1535){

    SecEner4637 = d4637(gen);

    energies.push_back(SecEner4637);

    if(2.1442 <= SecEner4637 && SecEner4637 <= 2.1462){
      
      SecEner2490 = d2490(gen);
      
      energies.push_back(SecEner2490);
      
      if(0.878 <= SecEner2490 && SecEner2490 <= 0.879){
        
        energies.push_back(d1611(gen));
      
      }
    }

    if(3.2259 <= SecEner4637 && SecEner4637 <= 3.2279){
      
      energies.push_back(d1410(gen));

    }

  }
    
  if(4.2111 <= PrimEner && PrimEner <= 4.2121){

    energies.push_back(d4578(gen));
    
    SecEner2490 = d2490(gen);
    
    energies.push_back(SecEner2490);
    
    if(0.878 <= SecEner2490 && SecEner2490 <= 0.879){
      energies.push_back(d1611(gen));
    }
  }

  if(4.34205 <= PrimEner && PrimEner <= 4.34255){

    energies.push_back(d4448(gen));

    SecEner2490 = d2490(gen);

    energies.push_back(SecEner2490);

    if(0.878 <= SecEner2490 && SecEner2490 <= 0.879){
      energies.push_back(d1611(gen));
    }
  }
  
  if(4.81005 <= PrimEner && PrimEner <= 4.81055){

    energies.push_back(d3981(gen));

  }

  if(4.85105 <= PrimEner && PrimEner <= 4.85255){
    
    energies.push_back(d3938(gen));

  }

  if(5.27215 <= PrimEner && PrimEner <= 5.27305){

    SecEner3518 = d3518(gen);
    energies.push_back(SecEner3518);

    if(1.02645 <= SecEner3518 && SecEner3518 <= 1.02695){

      SecEner2490 = d2490(gen);

      energies.push_back(SecEner2490);

      if(0.878 <= SecEner2490 && SecEner2490 <= 0.879){
        energies.push_back(d1611(gen));
      }
    }

    if(2.1073 <= SecEner3518 && SecEner3518 <= 2.1077){
      
      energies.push_back(d1410(gen));
      
    }
  }
  
  if(6.2994 <= PrimEner && PrimEner <= 6.3){
    
    SecEner2490 = d2490(gen);
    
    energies.push_back(SecEner2490);
    
    if(0.878 <= SecEner2490 && SecEner2490 <= 0.879){
      energies.push_back(d1611(gen));
      } 
  }
  
  }

  G4double sum=0.0;

  for( G4int j=0; j<energies.size(); j++ ){
    sum =  sum + energies[j];
    }

   G4cout << "Energy sum is: " << sum << G4endl;
   G4cout << "Multiplicity is: " << energies.size() << G4endl;

  for( G4int i=0; i<energies.size(); i++ ){
  
    G4cout << "Energia da partícula é: " << energies[i] << G4endl;


    /*** Generate random angular distribution here ****/
    G4double theta, phi;
    
    phi = twopi*G4UniformRand();
    theta = twopi*G4UniformRand() - pi;
    /*************************************************/

    G4cout << "Momentum direction is: " << sin(theta)*cos(phi) << " " 
                                        << sin(theta)*sin(phi) << " " 
                                        << cos(theta)          << " "
                                        << sqrt(pow(sin(theta)*cos(phi),2) + pow(sin(theta)*sin(phi),2) 
                                           + pow(cos(theta),2)) << G4endl;

    if(NBverbose > 1){
    G4cout << x << " "
           << y << " "
           << z << " "
           << sin(theta)*cos(phi) << " "
           << sin(theta)*sin(phi) << " " 
           << cos(theta) << " "
           << time << " New energy is: " 
           << energies[i] << G4endl;
    }
  
    // Create Primary Photons
    G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    G4PrimaryParticle* particle1 = new G4PrimaryParticle(particleDefinition);
    particle1->SetMomentumDirection({sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta)});    
 
    particle1->SetKineticEnergy(energies[i] /* ener */);
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
    particleNCap->SetEnergy  (  energies[i]  /* ener */       );

    // add initial photons from Neutron Capture
    mc_truth_manager->AddInitialGeneratorParticle(particleNCap);
  }

  if(vertexA->GetNumberOfParticle() == 0) {
    G4Exception("NeutronBackground::GeneratePrimaryVertex", "NB05",RunMustBeAborted, "Vertex with no particles! Probabaly you reached the end-of-file.");
    return;
  }
  anEvent->SetEventID(Evt);
  anEvent->AddPrimaryVertex(vertexA);

  energies.clear();
}