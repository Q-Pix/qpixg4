// -----------------------------------------------------------------------------
//  G4_QPIX | SteppingAction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
//
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

#include "SteppingAction.h"

#include "AnalysisData.h"
#include "AnalysisManager.h"
#include "G4VProcess.hh"

#include <stdlib.h> 
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "ConfigManager.h"

SteppingAction::SteppingAction(): G4UserSteppingAction()
{
}


SteppingAction::~SteppingAction()
{
}


void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4bool nCap_gamma = ConfigManager::GetGetPhotons();
  G4bool nCap_elec = ConfigManager::GetGetElectrons();

  G4double flagProcess=0.;

  G4String procName;
  G4int pdgCode, ev;

  const G4Event *evt;

  evt      = G4RunManager::GetRunManager()->GetCurrentEvent();
  pdgCode  = step->GetTrack()->GetDefinition()->GetPDGEncoding();
  procName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  
  // -----------------------------------------------------------------------------
  //   FOR NEUTRON BACKGROUND STUDIES ONLY!
  // -----------------------------------------------------------------------------
  //   If simulating primary neutrons (
  //   ./app/G4_QPIX ../macros/neutron_surf.mac | awk '/NCapGamma:/ {print $4,$5,$6,$7,$8,$9,$10}' > ../output/photon_ncap.txt
  //   and 
  //   ./app/G4_QPIX ../macros/Template_Neutron_Background.mac 
  //   )
  //   Use the following piece of code to get the information
  //   Of photons and secondary electrons from neutron capture
  // -----------------------------------------------------------------------------
 

  if(nCap_gamma){
  if(pdgCode == 2112 && procName == "nCapture"){

    G4double xph,yph,zph;

    const std::vector<const G4Track*> secondaries = *step->GetSecondaryInCurrentStep();

    for(auto & sec : secondaries){

      xph=sec->GetPosition().x()/mm;
      yph=sec->GetPosition().y()/mm;
      zph=sec->GetPosition().z()/mm;

      // Making sure photons are being collected
      // And we are getting them inside the detector
      if(sec->GetDefinition()->GetPDGEncoding() == 22){
          if(step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "detector.physical"){
          
            G4cout << "NCapGamma: "
                   << xph                                                    << " " 
                   << yph                                                    << " "
                   << zph                                                    << " " 
                   << sec->GetMomentumDirection().x()                        << " "
                   << sec->GetMomentumDirection().y()                        << " "
                   << sec->GetMomentumDirection().z()                        << " "
                   << sec->GetGlobalTime()/ns                                << G4endl;
          }
      }
    }
  }
  }

  if(nCap_elec){
    if(abs(pdgCode) == 11){
    

      G4double xe,ye,ze;

      if(step->GetTrack()->GetCurrentStepNumber()==1){

        xe= step->GetPreStepPoint()->GetPosition().x()/mm;
        ye= step->GetPreStepPoint()->GetPosition().y()/mm;
        ze= step->GetPreStepPoint()->GetPosition().z()/mm;

        // Making sure electrons are being collected
        // And we are getting them inside the detector
        if(step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "detector.physical"){
                  
                  std::cout << "NCapElec: "
                         << evt->GetEventID()                                    << " " 
                         << pdgCode                                              << " " 
                         << step->GetPreStepPoint()->GetKineticEnergy()/eV       << " "
                         << xe                                                   << " " 
                         << ye                                                   << " " 
                         << ze                                                   << std::endl;
        } 
      }
    }
  }
  
  // -----------------------------------------------------------------------------
  // NEUTRON BACKGROUND code ENDS HERE!
  // -----------------------------------------------------------------------------

    AnalysisManager::Instance();

    if (step->GetPostStepPoint()->GetProcessDefinedStep() != 0){
      event.AddProcess(step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
    } else {
      event.AddProcess("User Limit");
    }

}
