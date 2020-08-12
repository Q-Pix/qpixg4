// -----------------------------------------------------------------------------
//  G4Basic | TrackingSD.cpp
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
  hc_(nullptr)
{
  collectionName.insert(hc_name);
  //SteppingAction::OpenFile();
  //Data_File.open("/Users/austinmcdonald/projects/Q-pix/test.txt");

  msg_ = new G4GenericMessenger(this, "/Inputs/");
  msg_->DeclareMethod("output_file", &TrackingSD::OpenFile, "");

}


TrackingSD::~TrackingSD()
{
  Data_File.close();
}


void TrackingSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  hc_ = new TrackingHitsCollection(SensitiveDetectorName, collectionName[0]);
  // Add this collection in hce
  G4int hcid = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcid, hc_);
}


G4bool TrackingSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  //std::vector< std::vector<double> > Event_Vector;
  G4double edep = aStep->GetTotalEnergyDeposit();

  if (edep==0.) return false;

  TrackingHit* newHit = new TrackingHit();

  newHit->SetEdep(edep);
  newHit->SetPosition(aStep->GetPostStepPoint()->GetPosition());

  hc_->insert(newHit);

  // my dirty vector
  Tmp_Vector.push_back(aStep->GetPostStepPoint()->GetPosition()[0] + (1.0/2)*m);
  Tmp_Vector.push_back(aStep->GetPostStepPoint()->GetPosition()[1] + (1.0/2)*m);
  Tmp_Vector.push_back(aStep->GetPostStepPoint()->GetPosition()[2] + (5.0/2)*m);
  Tmp_Vector.push_back(edep);

  Event_Vector.push_back(Tmp_Vector);
  Tmp_Vector.clear();
  Data_File <<  Acounter  <<"\t"<< 
    aStep->GetPostStepPoint()->GetPosition()[0] + (1.0/2)*m <<"\t"<< 
    aStep->GetPostStepPoint()->GetPosition()[1] + (1.0/2)*m <<"\t"<<
    aStep->GetPostStepPoint()->GetPosition()[2] + (5.0/2)*m <<"\t"<< 
    aStep->GetTotalEnergyDeposit()              <<"\t"<< std::endl;
  //G4cout << G4endl << "Austin " << aStep->GetPostStepPoint()->GetPosition() << G4endl;

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


void TrackingSD::EndOfEvent(G4HCofThisEvent*)
{
  // G4int nofHits = hc_->entries();
  // G4cout
  //   << G4endl
  //   << "-------->Hits Collection: in this event there are " << nofHits
  //   << " hits in the tracker." << G4endl;
  //   

  //   G4cout<< Acounter << G4endl;
  //   Acounter +=1;

  // for (int i = 0; i < nofHits; i++) 
  // { 
  //   //G4cout<< Event_Vector.at(i)<< " ";
  //   for (int j = 0; j < 4; j++)
  //   { 
  //     G4cout<< Event_Vector[i][j]<< " "; 
  //   } 
  //   G4cout<< "\n"; 
  // }

}


void TrackingSD::OpenFile(G4String filename)
{
  Data_File.open(filename);
  //Data_File << "EventID" <<"\t"<< "xpos" <<"\t"<< "ypos" <<"\t"<< "zpos" <<"\t"<< "hitE" <<"\t"<< "stepL" << std::endl;
}
