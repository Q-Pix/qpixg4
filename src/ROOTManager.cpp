//
// Created by ilker on 12/20/21.
//


// Class Includes
#include "ROOTManager.h"

// Package Includes
#include "ConfigManager.h"

// ROOT Includes
#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"

// Geant4 includes
#include "globals.hh"

// System Includes
#include <iostream>
#include <string>







ROOTManager * ROOTManager::instance_=0;

//--------------------------------
ROOTManager::ROOTManager()
  : inputFile_(ConfigManager::GetInputFile()),
  particleType_(ConfigManager::GetParticleType()),
  genieFormat_(ConfigManager::GetGenieFormat())
{
}


//--------------------------------
ROOTManager::~ROOTManager() {
}
//--------------------------------
ROOTManager * ROOTManager::Instance()
{
  if (instance_ == 0) instance_ = new ROOTManager();
  return instance_;
}



G4int ROOTManager::Initialize() {
  
  genieFormat_.toLower();

  if (genieFormat_ == "rootracker" || genieFormat_ == "grootracker") {
    treeName_ = "gRooTracker";
  } else {
    G4cerr << "The genie format entered: '" << genieFormat_ << "' is not currently supported." << G4endl
           << "Please make the necessary adjustments in ROOTManager.cpp and ROOTManager.h and try again." << G4endl
           << G4endl
           << "Current list of supported genie formats:" << G4endl
           << " - rootracker" << G4endl;
    abort();
  } 
 
  if (!inputFile_.empty())
  {
    isInitialized=0;
    std::cout << "Configuring to Read From ROOT File..." << std::endl;
    // Reading the root file to extract the info
    if (tree_ == 0) {
      G4cout << "Test 1" << G4endl;
      f_ = (TFile*)gROOT->GetListOfFiles()->FindObject(inputFile_);
      G4cout << "Test 2" << G4endl;
      if (!f_ || !f_->IsOpen()) {
        G4cout << "Test 2.0.0" << G4endl;
        f_ = new TFile(inputFile_);
        G4cout << "Test 2.0.1" << G4endl;
      }
      if(f_->IsZombie()){
        G4cout << "Test 2.1.0" << G4endl;
        return isInitialized ;
      }
      G4cout << "Test 2.2" << G4endl;
      f_->GetObject(treeName_,tree_);
    }
    isInitialized=1;
    G4cout << "Test 3" << G4endl;
    return isInitialized;
  }
  else {
    std::cout << "Generating from ROOT is not selected!  Continuing..."<< std::endl;
    isInitialized=0;
    return isInitialized;
  }

}


void ROOTManager::SetBranches() {
  // Creating Root Branches
  if (treeName_=="gRooTracker") {
    tree_->SetBranchAddress("EvtNum",           &event_,            &b_event);
    //tree_->SetBranchAddress("EvtFlags",        flags_,            &b_flags);
    //tree_->SetBranchAddress("EvtCode",         code_,             &b_code);
    //tree_->SetBranchAddress("EvtXSec",         xsec_,             &b_xsec);
    //tree_->SetBranchAddress("EvtDXSec",        dxsec_,            &b_dxsec);
    tree_->SetBranchAddress("EvtWght",          &weight_,           &b_weight);
    tree_->SetBranchAddress("EvtProb",          &prob_,             &b_prob);
    tree_->SetBranchAddress("EvtVtx",           &vtx_,              &b_vtx);
    tree_->SetBranchAddress("StdHepN",          &NParticles_,       &b_NParticles);
    tree_->SetBranchAddress("StdHepPdg",        &pdg_,              &b_pdg);
    tree_->SetBranchAddress("StdHepStatus",     &Status,            &brStatus   );
    //tree_->SetBranchAddress("StdHepRescat",    rescat_,           &b_rescat);
    //tree_->SetBranchAddress("StdHepX4",        X4_,               &b_X4);
    tree_->SetBranchAddress("StdHepP4",         &P4_,               &b_P4);
    //tree_->SetBranchAddress("StdHepPolz",      polz_,             &b_polz);
    tree_->SetBranchAddress("StdHepFm",         &FirstMother,	    &brFirstMother   );
    tree_->SetBranchAddress("StdHepLm",         &LastMother,	    &brLastMother   );
    tree_->SetBranchAddress("StdHepFd",         &FirstDaughter,     &brFirstDaughter   );
    tree_->SetBranchAddress("StdHepLd",         &LastDaughter,      &brLastDaughter   );
  }

}

Int_t ROOTManager::GetNEntries() {
  return tree_->GetEntriesFast();
}


void ROOTManager::Cd(){
  if(f_)
    f_->cd();
}


void ROOTManager::Close(){
  if(f_){
    this->Cd();
    f_->Close();
  }

}
