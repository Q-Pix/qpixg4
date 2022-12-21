// -----------------------------------------------------------------------------
//  G4QPIX | G4_QPIX.cpp
//
//  Main program of the G4QPIX detector simulation.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "ActionInitialization.h"
#include "ConfigManager.h"
#include "DetectorConstruction.h"
#include "EventAction.h"
#include "PrimaryGeneration.h"
#include "ROOTManager.h"
#include "RunAction.h"
#include "SteppingAction.h"
#include "TrackingAction.h"


#include "G4RunManagerFactory.hh"
#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include "G4VisExecutive.hh"
#include <G4UIExecutive.hh>
#include <FTFP_BERT_HP.hh>
#include <G4EmStandardPhysics_option4.hh>
#include <G4OpticalPhysics.hh>

#include "TROOT.h"

#include "Randomize.hh"
#include "time.h"
#include <string>
#include <typeinfo>


int main(int argc, char** argv)
{

  ConfigManager::Instance();
  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  //set random seed with system time
  G4long seed = time(NULL);
  CLHEP::HepRandom::setTheSeed(seed);



  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Construct the run manager and set the initialization classes
  auto* run_manager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  G4VModularPhysicsList* physics_list = new FTFP_BERT_HP();
  physics_list->ReplacePhysics(new G4EmStandardPhysics_option4());
  run_manager->SetUserInitialization(physics_list);

  run_manager->SetUserInitialization(new DetectorConstruction());

  run_manager->SetUserInitialization(new ActionInitialization());

  // Initialize visualization
  G4VisManager* vismgr = new G4VisExecutive();
  vismgr->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* uimgr = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if (!ui) {
    // batch mode
    // Execute Macro file using any aliases set above
    G4String command = "/control/execute ";
    G4String macro_name = argv[1];
    uimgr->ApplyCommand(command+macro_name);
  }
  else {
    // interactive mode
    uimgr->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program.

  delete vismgr;
  delete run_manager;
}
