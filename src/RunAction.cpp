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
#include "ConfigManager.h"
#include "MARLEYManager.h"
#include "MCTruthManager.h"
#include "ROOTManager.h"
#include "DetectorConstruction.h"
#include "GENIEManager.h"

// GEANT4 includes
#include "G4Box.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4VUserDetectorConstruction.hh"

// C++ includes
#include <filesystem>


RunAction::RunAction(): G4UserRunAction(), multirun_(false),TreeName_("event_tree")
{
    runManager=G4RunManager::GetRunManager();
}


RunAction::~RunAction()
{
    delete ROOTManager::Instance();
}


void RunAction::BeginOfRunAction(const G4Run* g4run)
{
    ConfigManager::Instance();
    //ConfigManager::Print();

    inputFile_ = ConfigManager::GetInputFile();
    outputFile_ = ConfigManager::GetOutputFile();
    marleyJson_ = ConfigManager::GetMarleyJson();
    generator_ = ConfigManager::GetGenerator();
    genieFormat_ = ConfigManager::GetGenieFormat();
    multirun_ = ConfigManager::GetMultirun();
    particleType_ = ConfigManager::GetParticleType();



    particleType_.toLower();
    generator_.toLower();
    genieFormat_.toLower();

    //ConfigManager::Print();

    if (generator_ == "marley") {

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
                std::cout<<"Number of Events to Process -->"<<g4run->GetNumberOfEventToBeProcessed()<<std::endl;

            }

        }else
             G4Exception("[RunAction]","[BeginOfRunAction]",G4ExceptionSeverity::FatalException ,"RootManager is not properly initialized. Check to see if the following file exist /Inputs/ReadFrom_Root_Path in macros/ROOTRead.macro   ") ;

    }

    if(!marleyJson_.empty()){
        // get MARLEY manager
        MARLEYManager * marley_manager = MARLEYManager::Instance();
        // configure and create MARLEY generator
        marley_manager->Initialize();
    }



    std::string root_output_path = root_output_path_;

    if (multirun_)
    {

        std::ostringstream ss;
        ss << std::setw(4) << std::setfill('0') << g4run->GetRunID();
        std::string runStr_(ss.str());
        G4String parentPath_ = outputFile_(0, outputFile_.last('/'));
        G4String baseName_ = outputFile_(outputFile_.last('/')+1, outputFile_.length());
        G4String stem_ = baseName_(0, baseName_.last('.'));
        G4String extension_ = baseName_(baseName_.last('.'), baseName_.length());
        
        root_output_path = parentPath_;
        root_output_path += "/";
        root_output_path += stem_;
        root_output_path += "_";
        root_output_path += runStr_;
        root_output_path += extension_;

    }

    // get run number
    AnalysisManager * analysis_manager = AnalysisManager::Instance();
    // if(particle_type_ > 0)
    //     analysis_manager->SetParticleID(particle_type_);
    analysis_manager->Book(root_output_path);
    event.SetRun(g4run->GetRunID());
    // if we've passed a particle type, then activate and set the branch

    // reset event variables
    event.EventReset();

    // get MC truth manager
    MCTruthManager * mc_truth_manager = MCTruthManager::Instance();

    // reset event in MC truth manager
    mc_truth_manager->EventReset();
    }
}


void RunAction::EndOfRunAction(const G4Run* g4run)
{
    // check the ROOTManager to add extra meta-data. this is a placeholder of a janky configmanager
    ROOTManager *rootManager=ROOTManager::Instance();
    // instantiate classes to allow for messengers
    AnalysisManager * analysis_manager = AnalysisManager::Instance();

    // These are filled within the generate primaries only when a non-background
    // event is created and there is metadata to add if the root manager has a
    // non-default fsPdg number
    if(rootManager->fsPdg != 0){
        analysis_manager->FillROOTMeta(
        rootManager->axis_x_,
        rootManager->axis_y_,
        rootManager->axis_z_,
        rootManager->xpos,
        rootManager->ypos,
        rootManager->zpos,
        rootManager->nEvt,
        rootManager->fsPdg,
        rootManager->fsEnergy,
        rootManager->fsEvt,
        rootManager->fsFileno,
        rootManager->fsFHC,
        rootManager->fsRun,
        rootManager->nFS,
        rootManager->fsLepKE,
        // add the hadron values
        rootManager->hadTot_,
        rootManager->hadPip_,
        rootManager->hadPim_,
        rootManager->hadPi0_,
        rootManager->hadP_,
        rootManager->hadN_,
        rootManager->hadOther_,
        // all of the other variables tracked in the GENIE file
        analysis_manager->GetEnergy()
        );
    }
    rootManager->Close();

    GENIEManager::Instance();


    inputFile_ = ConfigManager::GetInputFile();
    outputFile_ = ConfigManager::GetOutputFile();
    marleyJson_ = ConfigManager::GetMarleyJson();
    generator_ = ConfigManager::GetGenerator();
    genieFormat_ = ConfigManager::GetGenieFormat();
    multirun_ = ConfigManager::GetMultirun();
    particleType_ = ConfigManager::GetParticleType();

    // save detector dimensions as metadata
    if (G4Threading::IsMasterThread()){
      analysis_manager->FillMetadata(); // TODO
    }


    // save run to ROOT file
    analysis_manager->Save();
}

