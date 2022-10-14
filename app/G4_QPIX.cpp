// -----------------------------------------------------------------------------
//  G4QPIX | G4_QPIX.cpp
//
//  Main program of the G4QPIX detector simulation.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "DetectorConstruction.h"
#include "PrimaryGeneration.h"
#include "RunAction.h"
#include "EventAction.h"
#include "TrackingAction.h"
#include "SteppingAction.h"
#include "../src/ROOTManager.h"
#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>
#include <FTFP_BERT_HP.hh>
#include <G4EmStandardPhysics_option4.hh>
#include <G4OpticalPhysics.hh>


#include "Randomize.hh"
#include "time.h"
#include <getopt.h>


void PrintHelp(){
    G4cerr << "\nUsage: ./build/app/G4_QPIX [-i <root_file>] [-g <generator>] [-f <root_file_format>] -m <macro>\n"  << G4endl;
    G4cerr << "Available options: " <<G4endl;
    G4cerr << "-i --input --> input .root file  " <<G4endl;
    G4cerr << "-g --generator --> specify which generator created the file   " <<G4endl;
    G4cerr << "-f --format  --> specify format of the input file " <<G4endl;
    G4cerr << "-m --macro  --> specify the macro file to use (REQUIRED) " <<G4endl;
    G4cerr << "-h --help --> Help" <<G4endl;
    exit(EXIT_FAILURE);
}


int main(int argc, char** argv)
{

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
  G4RunManager* run_manager = new G4RunManager();

  G4VModularPhysicsList* physics_list = new FTFP_BERT_HP();
  physics_list->ReplacePhysics(new G4EmStandardPhysics_option4());
  run_manager->SetUserInitialization(physics_list);

  run_manager->SetUserInitialization(new DetectorConstruction());

  run_manager->SetUserAction(new PrimaryGeneration());
  run_manager->SetUserAction(new RunAction());
  run_manager->SetUserAction(new EventAction());
  run_manager->SetUserAction(new TrackingAction());
  run_manager->SetUserAction(new SteppingAction());

  // Initialize visualization
  G4VisManager* vismgr = new G4VisExecutive();
  vismgr->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* uimgr = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if (!ui) {
    // batch mode
    const char* const short_opts = "i::g::f::m:h";
    const option long_options[] = {
      {"input",       optional_argument,            nullptr,   'i'},
      {"generator",   optional_argument,            nullptr,   'g'},
      {"format",      optional_argument,            nullptr,   'f'},
      {"macro",       required_argument,            nullptr,   'm'},
      {"help",        no_argument,                  nullptr,   'h'},
      {0, 0, 0, 0};
    };

    string macro_name = "";

    while (1) {

      const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

      if (opt == -1)
        break;

      switch (opt) {

        case 'i':
          uimgr->SetAlias("ROOT_FILE "+optarg);
          break;

        case 'g':
          uimgr->SetAlias("GENERATOR "+optarg);
          break;

        case 'f':
          uimgr->SetAlias("INPUT_FORMAT "+optarg);
          break;

        case 'm':
          macro_name = optarg;
          break;
 
        case 'h':
          PrintHelp();
          break; 

        case '?':
          PrintHelp();
          break;

        default:
          abort();
      }
    }

    // Still allow the submission of jobs using the old format.
    // If there are no options specified, but still arguments, assume the old method
    if (optind < argc && argc - optind == 1) {
      printf("Macro file entered old way. Using macro file: ");
      while (optind < argc) {
        printf("%s ", argv[optind++]);
        macro_name = argv[optind++];
      }
      printf("\n");
    }
  
    // Check that there are no extra non-option arguments. If so, print help message and abort.
    if (optind < argc && argc - optind > 1) {
      printf("Too many non-option arguments entered! Non-option arguments entered");
      while (optind < argc) {
        printf("%s ", argv[optind++]);
      }
      printf("\n");
      printf("You must either submit batch job using options (see help) or in the old way: ./Build/G4_QPIX [insert_macro_file]");
      PrintHelp();
      abort()
    }

    // Execute Macro file using any aliases set above
    G4String command = "/control/execute ";
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
