// -----------------------------------------------------------------------------
//  G4Basic | RunAction.h
//
//
//   * Author: Everybody is an author!
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#include "RunAction.h"

// Q-Pix includes
#include "AnalysisManager.h"
#include "MARLEYManager.h"
#include "MCTruthManager.h"
#include "ROOTManager.h"

// GEANT4 includes
#include "G4Box.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

// C++ includes
#include <filesystem>


RunAction::RunAction(): G4UserRunAction(), multirun_(false),TreeName_("event_tree")
{
    messenger_ = new G4GenericMessenger(this, "/Inputs/");
    messenger_->DeclareProperty("root_output", root_output_path_,
                                "path to output ROOT file");
    messenger_->DeclareProperty("MARLEY_json", marley_json_,
                                "MARLEY configuration file");
    messenger_->DeclareProperty("ReadFrom_Root_Path", ReadFrom_Root_,
                                "Root File to Read");
    messenger_->DeclareProperty("multirun", multirun_,
                                "Multiple runs");
    messenger_->DeclareProperty("TreeName", TreeName_,
                                "RootTree Name if it is defined different");
    messenger_->DeclareProperty("RadioParticleID", particle_type_,
                                "Int ID for Decay Particle");
    runManager=G4RunManager::GetRunManager();
}


RunAction::~RunAction()
{
    delete messenger_;
    delete ROOTManager::Instance();
}


void RunAction::BeginOfRunAction(const G4Run* run)
{

    G4cout << "RunAction::BeginOfRunAction: Run #" << run->GetRunID() << " start." << G4endl;

    //Get Root Manager
    if(!ReadFrom_Root_.empty()){
        ROOTManager *rootManager=ROOTManager::Instance();
        if(rootManager->Initialize(ReadFrom_Root_,TreeName_)){
            rootManager->SetBranches();
            G4int NumberEventsInTheFile=(G4int)rootManager->GetNEntries();

            //Get Current RunManager to change number events if they exceed the numberof events in the file.


            // Replacing the Number of Current Events if it is 0 or set higher than the events in the root file
            if(runManager->GetNumberOfEventsToBeProcessed()>NumberEventsInTheFile or runManager->GetNumberOfEventsToBeProcessed()==228){
                std::cout<<"Overwrting number of events to proccess to " << NumberEventsInTheFile <<" ..."<<std::endl;
                runManager->SetNumberOfEventsToBeProcessed(NumberEventsInTheFile);
                std::cout<<"Number of Events to Process -->"<<run->GetNumberOfEventToBeProcessed()<<std::endl;

            }

        }else
             G4Exception("[RunAction]","[BeginOfRunAction]",G4ExceptionSeverity::FatalException ,"RootManager is not properly initialized. Check to see if the following file exist /Inputs/ReadFrom_Root_Path in macros/ROOTRead.macro   ") ;

    }

    if(!marley_json_.empty()){
    // get MARLEY manager
    MARLEYManager * marley_manager = MARLEYManager::Instance();
    // configure and create MARLEY generator
    marley_manager->Initialize(marley_json_);
    }



    std::string root_output_path = root_output_path_;

    if (multirun_)
    {

        std::ostringstream ss;
        ss << std::setw(4) << std::setfill('0') << run->GetRunID();
        std::string run_str(ss.str());

        // G4cout << "run_str: " << run_str << G4endl;

        std::filesystem::path path = static_cast<std::string> (root_output_path_);

        // G4cout << "root_name:      " << path.root_name()      << G4endl;
        // G4cout << "root_directory: " << path.root_directory() << G4endl;
        // G4cout << "root_path:      " << path.root_path()      << G4endl;
        // G4cout << "relative_path:  " << path.relative_path()  << G4endl;
        // G4cout << "parent_path:    " << path.parent_path()    << G4endl;
        // G4cout << "filename:       " << path.filename()       << G4endl;
        // G4cout << "stem:           " << path.stem()           << G4endl;
        // G4cout << "extension:      " << path.extension()      << G4endl;

        std::string parent_path = path.parent_path();
        std::string stem = path.stem();
        std::string extension = path.extension();

        // G4cout << "parent_path: " << parent_path << G4endl;
        // G4cout << "stem:        " << stem        << G4endl;
        // G4cout << "extension:   " << extension   << G4endl;

        root_output_path = parent_path + "/" + stem + "_" + run_str + extension;

        // G4cout << "root_output_path: " << root_output_path << G4endl;

    }

    // get run number
    AnalysisManager * analysis_manager = AnalysisManager::Instance();
    if(particle_type_ > 0)
        analysis_manager->SetParticleID(particle_type_);
    analysis_manager->Book(root_output_path);
    analysis_manager->SetRun(run->GetRunID());
    // if we've passed a particle type, then activate and set the branch

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
    ROOTManager *rootManager=ROOTManager::Instance();
    rootManager->Close();
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

