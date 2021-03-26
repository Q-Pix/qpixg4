// -----------------------------------------------------------------------------
//  G4_QPIX | TrackingSD.cpp
//
//  TODO: Class description
//   * Author: Justo Martin-Albo
//   * Creation date: 13 Feb 2019
// -----------------------------------------------------------------------------

#include "TrackingSD.h"
#include "TrackingHit.h"

// Q-Pix includes
#include "MCTruthManager.h"
#include "MCParticle.h"

// GEANT4 includes
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

// C++ includes
#include <vector>


TrackingSD::TrackingSD(const G4String& sd_name, const G4String& hc_name):
  G4VSensitiveDetector(sd_name),
  Event_Cutoff_(0.0)
  // hc_(nullptr)
{
  collectionName.insert(hc_name);

  msg_ = new G4GenericMessenger(this, "/Supernova/", "Control commands of the supernova generator.");
  msg_->DeclareProperty("Event_Cutoff", Event_Cutoff_,  "window to simulate the times").SetUnit("ns");

}


TrackingSD::~TrackingSD()
{
  delete msg_;
}


// void TrackingSD::Initialize(G4HCofThisEvent* hce)
// {
//   // Create hits collection
//   hc_ = new TrackingHitsCollection(SensitiveDetectorName, collectionName[0]);
//   // Add this collection in hce
//   G4int hcid = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
//   hce->AddHitsCollection(hcid, hc_);
// }


G4bool TrackingSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();

  // if (edep==0.) return false;
  if (edep < 1. *keV ) return false;

  if (Event_Cutoff_ != 0.0)
  {
    // G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint* preStepPoint = aStep->GetPostStepPoint();
    G4double hitTime = preStepPoint->GetGlobalTime(); 
    // if (hitTime<0. || hitTime>Event_Cutoff_) return false;
    if (hitTime>Event_Cutoff_) return false;
  }

  // TrackingHit* newHit = new TrackingHit();

  // newHit->SetEdep(edep);
  // newHit->SetPosition(aStep->GetPostStepPoint()->GetPosition());

  // hc_->insert(newHit);

  //---------------------------------------------------------------------------
  // begin add hit to MCParticle
  //---------------------------------------------------------------------------

  // get MC truth manager
  MCTruthManager * mc_truth_manager = MCTruthManager::Instance();

  // get MC particle
  MCParticle * particle = mc_truth_manager->GetMCParticle(aStep->GetTrack()->GetTrackID());

  // add hit to MC particle
  particle->AddTrajectoryHit(aStep);

  //---------------------------------------------------------------------------
  // end add hit to MCParticle
  //---------------------------------------------------------------------------

  return true;
}


// void TrackingSD::EndOfEvent(G4HCofThisEvent*)
// {
//   G4int nofHits = hc_->entries();
//   G4cout
//     << G4endl
//     << "-------->Hits Collection: in this event there are " << nofHits
//     << " hits in the tracker." << G4endl;
// }

