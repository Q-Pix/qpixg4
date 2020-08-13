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
    // save run to ROOT file
    AnalysisManager * analysis_manager = AnalysisManager::Instance();
    analysis_manager->Save();
}
