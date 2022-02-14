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
    G4cerr << "\nUsage: ./build/app/G4_QPIX [-i/b] [-n number] <init_macro>\n"  << G4endl;
    G4cerr << "Available options: " <<G4endl;
    G4cerr << "-n --nevents --> number of events to simulate " <<G4endl;
    G4cerr << "-i --interactive --> Interactive Mode   " <<G4endl;
    G4cerr << "-b --batch  --> No Visual Mode " <<G4endl;
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
  if(argc<2) PrintHelp();

  G4bool interactive=false;
  G4int nevents=0;
  static struct option long_options[]={
          {"interactive",no_argument,0,'i'},
          {"batch",no_argument,0,'b'},
          {"interactive",no_argument,0,'i'},
          {"nevents",no_argument,0,'n'},
          {0,0,0,0}
  };

    int opt;
  while(true){
      opterr=0;
      opt=getopt_long(argc, argv, "bin:", long_options, 0);
      if (opt==-1) break; // Exit if we are done reading options

      switch (opt) {

          case 'i':
              interactive= true;
              break;

          case 'b':
              interactive= false;
              break;

          case 'n':
              nevents = atoi(optarg);
              break;

          case '?':
              break;

          case 'h':
              PrintHelp();
              break;
          default:
              abort();
      }
  }

  if(optind==argc) PrintHelp();
  G4String macfile=argv[optind];
  if(macfile=="") PrintHelp();


  // Construct the run manager and set the initialization classes
  G4RunManager* run_manager = new G4RunManager();

  G4VModularPhysicsList* physics_list = new FTFP_BERT_HP();
  physics_list->ReplacePhysics(new G4EmStandardPhysics_option4());
  physics_list->RegisterPhysics(new G4OpticalPhysics());;
  run_manager->SetUserInitialization(physics_list);

  run_manager->SetUserInitialization(new DetectorConstruction());

  run_manager->SetUserAction(new PrimaryGeneration());
  run_manager->SetUserAction(new RunAction());
  run_manager->SetUserAction(new EventAction());

  run_manager->SetUserAction(new TrackingAction());
  run_manager->SetUserAction(new SteppingAction());


  // Get the pointer to the User Interface manager
  G4UImanager* uimgr = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if (interactive) {       // interactive mode

      // Initialize visualization

      G4VisManager* vismgr = new G4VisExecutive();
      ui = new G4UIExecutive(argc, argv);
      vismgr->Initialize();
      uimgr->ApplyCommand("/control/execute cfg/init_vis.mac");
      G4String command = "/control/execute ";
      G4String fileName = macfile;
      uimgr->ApplyCommand(command+fileName);
      ui->SessionStart();
      delete ui;
      delete vismgr;
  }
  else {
      // batch mode
      run_manager->Initialize();
      G4String command = "/control/execute ";
      G4String fileName = macfile;
      uimgr->ApplyCommand(command+fileName);
      if(nevents!=0)
        run_manager->BeamOn(nevents);

  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program.

  delete run_manager;
  return EXIT_SUCCESS;

}
