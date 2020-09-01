// -----------------------------------------------------------------------------
//  G4Basic | RunAction.h
//
//
//   * Author: Justo Martin-Albo
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#include "RunAction.h"

// Q-Pix includes
#include "AnalysisManager.h"
#include "MARLEYManager.h"
#include "MCTruthManager.h"

// GEANT4 includes
#include "G4Box.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Run.hh"


RunAction::RunAction(): G4UserRunAction()
{
    messenger_ = new G4GenericMessenger(this, "/Inputs/");
    messenger_->DeclareProperty("root_output", root_output_path_,
                                "path to output ROOT file");
    messenger_->DeclareProperty("MARLEY_json", marley_json_,
                                "MARLEY configuration file");
}


RunAction::~RunAction()
{
    delete messenger_;
}


void RunAction::BeginOfRunAction(const G4Run* run)
{
    G4cout << "RunAction::BeginOfRunAction: Run #" << run->GetRunID() << " start." << G4endl;

    // get MARLEY manager
    MARLEYManager * marley_manager = MARLEYManager::Instance();
    // configure and create MARLEY generator
    marley_manager->Initialize(marley_json_);

    // get run number
    AnalysisManager * analysis_manager = AnalysisManager::Instance();
    analysis_manager->Book(root_output_path_);
    analysis_manager->SetRun(run->GetRunID());

    // reset event variables
    analysis_manager->EventReset();

    // get MC truth manager
    MCTruthManager * mc_truth_manager = MCTruthManager::Instance();

    // reset event in MC truth manager
    mc_truth_manager->EventReset();
}


void RunAction::EndOfRunAction(const G4Run*)
{
    // get analysis manager
    AnalysisManager * analysis_manager = AnalysisManager::Instance();

    // get detector dimensions
    G4LogicalVolume* detector_logic_vol
      = G4LogicalVolumeStore::GetInstance()->GetVolume("detector.logical");
    if (detector_logic_vol)
    {
      G4Box * detector_solid_vol
        = dynamic_cast<G4Box*>(detector_logic_vol->GetSolid());

      double const detector_length_x = detector_solid_vol->GetXHalfLength() * 2. / CLHEP::cm;
      double const detector_length_y = detector_solid_vol->GetYHalfLength() * 2. / CLHEP::cm;
      double const detector_length_z = detector_solid_vol->GetZHalfLength() * 2. / CLHEP::cm;

      // G4cout << "det. dim.: " << detector_length_x << " cm × "
      //                         << detector_length_y << " cm × "
      //                         << detector_length_z << " cm"
      //        << G4endl;

      // save detector dimensions as metadata
      analysis_manager->FillMetadata(detector_length_x,
                                     detector_length_y,
                                     detector_length_z);
    }

    // save run to ROOT file
    analysis_manager->Save();
}
