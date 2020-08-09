// -----------------------------------------------------------------------------
//  TrackingAction.cpp
//
//
//   * Author: Everybody is an author!
//   * Creation date: 4 August 2020
// -----------------------------------------------------------------------------

#include "TrackingAction.h"
#include "G4TrackingManager.hh"

#include <iostream>
#include <fstream>

TrackingAction::TrackingAction()
{}

TrackingAction::~TrackingAction()
{}

// void TrackingAction::PreUserTrackingAction(const G4Track* track)
void TrackingAction::PreUserTrackingAction(const G4Track*)
{
    // const G4DynamicParticle* dynamicParticle = track->GetDynamicParticle();
    // const G4ParticleDefinition* particleDefinition = track->GetDefinition();
    // const int pdgCode = particleDefinition->GetPDGEncoding();
    // const int trackID = track->GetTrackID();
    // const int parentID = track->GetParentID();
    // const double pdgCharge = particleDefinition->GetPDGCharge();
    // const double charge = dynamicParticle->GetCharge();
    // const int electrons = dynamicParticle->GetTotalOccupancy();
    // G4cout << "\n----------------------------------------------------------"
    //        << "\nTrackingAction::PreUserTrackingAction():"
    //        << "\n----------------------------------------------------------"
    //        << "\ntrackID:   " << trackID
    //        << "\nparentID:  " << parentID
    //        << "\npdgCode:   " << pdgCode
    //        << "\npdgCharge: " << pdgCharge
    //        << "\nCharge:    " << charge
    //        << "\nElectrons: " << electrons
    //        << "\n----------------------------------------------------------\n";
}

// void TrackingAction::PostUserTrackingAction(const G4Track* track)
void TrackingAction::PostUserTrackingAction(const G4Track*)
{
    // const G4DynamicParticle* dynamicParticle = track->GetDynamicParticle();
    // const G4ParticleDefinition* particleDefinition = track->GetDefinition();
    // const int pdgCode = particleDefinition->GetPDGEncoding();
    // const int trackID = track->GetTrackID();
    // const int parentID = track->GetParentID();
    // const double pdgCharge = particleDefinition->GetPDGCharge();
    // const double charge = dynamicParticle->GetCharge();
    // const int electrons = dynamicParticle->GetTotalOccupancy();
    // const std::string process = track->GetStep()->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    // G4cout << "\n----------------------------------------------------------"
    //        << "\nTrackingAction::PostUserTrackingAction():"
    //        << "\n----------------------------------------------------------"
    //        << "\ntrackID:   " << trackID
    //        << "\nparentID:  " << parentID
    //        << "\npdgCode:   " << pdgCode
    //        << "\npdgCharge: " << pdgCharge
    //        << "\nCharge:    " << charge
    //        << "\nElectrons: " << electrons
    //        << "\nprocess:   " << process
    //        << "\n----------------------------------------------------------\n";
}
