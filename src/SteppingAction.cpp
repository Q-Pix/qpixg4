// -----------------------------------------------------------------------------
//  G4_QPIX | SteppingAction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
//
//
//  GVS: 23 April 2024
//       ATTENTION! USE THE FOLLOWING COMMAND LINES WHEN 
//       RUNNING QPIXG4 FOR NEUTRON BACKGROUND STUDIES:
//       cd PathToQPIXG4directory/qpixg4
//       source run_neutron_background.sh
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
  G4int pdgCode;
  G4int isotope;

  const G4Event *evt;

  evt      = G4RunManager::GetRunManager()->GetCurrentEvent();
  pdgCode  = step->GetTrack()->GetDefinition()->GetPDGEncoding();
  procName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  
  // -----------------------------------------------------------------------------
  //   FOR NEUTRON BACKGROUND STUDIES ONLY!
  // -----------------------------------------------------------------------------
  //   If simulating primary neutrons,
  //   Use the following piece of code to get the information
  //   Of photons and secondary electrons from neutron capture
  // -----------------------------------------------------------------------------

/*   if(pdgCode == 2112){
    if(step->GetTrack()->GetParentID() == 0){
      if(step->GetTrack()->GetCurrentStepNumber()==1){
        if(procName != "Transportation"){
          if(step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "detector.physical"){
            G4cout << "It is a primary neutron!" << G4endl;
          }
        }
      }
    } 
  } */

  if(nCap_gamma){
    if(pdgCode == 2112 && procName == "nCapture"){
    
      G4int eventID; //, nphotons;
      G4double xph,yph,zph,tph; //,pxph,pyph,pzph;
      G4double sum = 0;
      G4String event_info;
      // G4String momdir_info = " ";
      // G4String append;

      const std::vector<const G4Track*> secondaries = *step->GetSecondaryInCurrentStep();

      // Making sure photons are being collected
      // And we are getting them inside the detector
      if(step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "detector.physical"){

        for(auto & sec : secondaries){
             if(sec->GetDefinition()->GetPDGEncoding() == 1000180410){isotope = 41;}
             if(sec->GetDefinition()->GetPDGEncoding() == 1000180370){isotope = 37;}
             if(sec->GetDefinition()->GetPDGEncoding() == 22){
                G4cout << "Energy is: " << sec->GetTotalEnergy() << G4endl;
                sum = sum + sec->GetTotalEnergy();
                
            }
        }

        G4cout << "Sum is: " << sum << G4endl;
        G4cout << "Multiplicity is: " << secondaries.size() - 1 << G4endl;

          // Position and time of the secondaries from the same vertex are the same.
          // These quantities will be saved once.
          eventID = evt->GetEventID();
          xph=secondaries.at(0)->GetPosition().x()/mm;
          yph=secondaries.at(0)->GetPosition().y()/mm;
          zph=secondaries.at(0)->GetPosition().z()/mm;
          tph=secondaries.at(0)->GetGlobalTime()/ns;

          event_info = "NCapGamma: " + std::to_string(isotope) + " "  + std::to_string(eventID) + " " 
                                 + std::to_string(xph) + " " + std::to_string(yph) + " " 
                                 + std::to_string(zph) + " " + std::to_string(tph) + " ";

          G4cout <<  event_info << G4endl; 
      }
    }
  }

      /* for(auto & sec : secondaries){

        // Making sure photons are being collected
        // And we are getting them inside the detector
        if(sec->GetDefinition()->GetPDGEncoding() == 22){
            if(step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "detector.physical"){
 
              pxph=sec->GetMomentumDirection().x();
              pyph=sec->GetMomentumDirection().y();
              pzph=sec->GetMomentumDirection().z();
              append = std::to_string(pxph) + " " 
                       + std::to_string(pyph) + " " 
                       + std::to_string(pzph) + " ";

            momdir_info = momdir_info + append;
            energy = energy + sec->GetTotalEnergy();
            }
        }
      } */

  if(nCap_elec){
    if(abs(pdgCode) == 11){
    

      G4double xe,ye,ze;

      //if(step->GetTrack()->GetCurrentStepNumber()==1){

        xe= step->GetPreStepPoint()->GetPosition().x()/mm;
        ye= step->GetPreStepPoint()->GetPosition().y()/mm;
        ze= step->GetPreStepPoint()->GetPosition().z()/mm;

        // Making sure electrons are being collected
        // And we are getting them inside the detector
        if(step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "detector.physical"){
                  
                  G4cout << "NCapElec: "
                         << evt->GetEventID()                                    << " " 
                         << pdgCode                                              << " " 
                         << step->GetPreStepPoint()->GetKineticEnergy()/eV       << " "
                         << xe                                                   << " " 
                         << ye                                                   << " " 
                         << ze                                                   << G4endl;
        } 
   //   }
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