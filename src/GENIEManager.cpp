///////////////////////////////////////////////////////////////////////////////
//
// GENIEManager.cpp
//
// Manages GENIE input to qpixg4
//
// Created by: Ilker 20122021
// Modified by: Dave Elofson 01092022
//
///////////////////////////////////////////////////////////////////////////////


// Class Includes
#include "GENIEManager.h"

// Package Includes
#include "ConfigManager.h"

// ROOT Includes
#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"

// Geant4 includes
#include "globals.hh"
#include "G4AutoLock.hh"

// System Includes
#include <iostream>
#include <string>


namespace {
  G4Mutex treeMutex = G4MUTEX_INITIALIZER;
}




GENIEManager * GENIEManager::instance_=0;

//--------------------------------
GENIEManager::GENIEManager()
  : event_(0), NParticles_(0), f_(0), tree_(0), xsec_(0.),
  dxsec_(0.), weight_(0.), prob_(0.), 
  NParticles(0),
  inputFile_(""), particleType_(""), genieFormat_(""), treeName_(""),
  isInitialized(0)
{
    //std::fill_n(pdg_, sizeof(pdg_)/sizeof(G4int), 0);
    //std::fill_n(vtx_, sizeof(vtx_)/sizeof(G4double), 0);
    //std::fill_n(X4_, sizeof(X4_)/sizeof(G4double), 0);
    //std::fill_n(P4_, sizeof(P4_)/sizeof(G4double), 0);
    //std::fill_n(polz_, sizeof(polz_)/sizeof(G4double), 0);
    //std::fill_n(idx, sizeof(idx)/sizeof(G4int), 0);
    //std::fill_n(Status, sizeof(Status)/sizeof(G4int), 0);
    //std::fill_n(rescat_, sizeof(rescat_)/sizeof(G4int), 0);
    //std::fill_n(FirstMother, sizeof(FirstMother)/sizeof(G4int), 0);
    //std::fill_n(LastMother, sizeof(LastMother)/sizeof(G4int), 0);
    //std::fill_n(FirstDaughter, sizeof(FirstDaughter)/sizeof(G4int), 0);
    //std::fill_n(LastDaughter, sizeof(LastDaughter)/sizeof(G4int), 0);
    
}


//--------------------------------
GENIEManager::~GENIEManager() {
}
//--------------------------------
GENIEManager * GENIEManager::Instance()
{
  if (instance_ == 0) instance_ = new GENIEManager();
  return instance_;
}



G4int GENIEManager::Initialize() {
  genieFormat_ = ConfigManager::GetGenieFormat();
  inputFile_ = ConfigManager::GetInputFile(); 
  particleType_ = ConfigManager::GetParticleType();
 
  genieFormat_.toLower();

  G4AutoLock treeLock(&treeMutex);

  if (genieFormat_ == "rootracker" || genieFormat_ == "grootracker") {
    treeName_ = "gRooTracker";
  } else {
    G4cerr << "The genie format entered: '" << genieFormat_ << "' is not currently supported." << G4endl
           << "Please make the necessary adjustments in GENIEManager.cpp and GENIEManager.h and try again." << G4endl
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
      f_ = (TFile*)gROOT->GetListOfFiles()->FindObject(inputFile_);
      if (!f_ || !f_->IsOpen()) {
        f_ = new TFile(inputFile_);
      }
      if(f_->IsZombie()){
        return isInitialized ;
      }
      f_->GetObject(treeName_,tree_);
      if (!(tree_ == 0)) {
        this->SetBranches();
      } else {
        G4cerr << "tree_ is a null pointer" << G4endl;
        abort;
      }
    }
    isInitialized=1;
    return isInitialized;
  }
  else {
    std::cout << "Generating from ROOT is not selected!  Continuing..."<< std::endl;
    isInitialized=0;
    return isInitialized;
  }

}


void GENIEManager::SetBranches() {
  if (!(tree_ == 0)){
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
  else{
    G4cerr << "tree_ is a null pointer" << G4endl;
    abort;
  }


}

Int_t GENIEManager::GetNEntries() {
  return tree_->GetEntriesFast();
}


void GENIEManager::Cd(){
  if(f_)
    f_->cd();
}


void GENIEManager::SaveMomentum(){
  std::vector<G4double> px, py, pz, e;
  for (int i=0; i<this->GetNEntries(); i++){
     tree_->GetEntry(i);
     px.push_back(P4_[i][0]);
     py.push_back(P4_[i][1]);
     pz.push_back(P4_[i][2]);
     e.push_back(P4_[i][3]);
  }

  TFile * fout = new TFile("momentum_report.root","recreate");
  TTree * p4 = new TTree("p4", "split momentums");
  p4->Branch("px",&px);
  p4->Branch("py",&py);
  p4->Branch("pz",&pz);
  p4->Branch("e",&e);

  p4->Fill();

  fout->Write();

  delete fout; 

}


void GENIEManager::Close(){
  if(f_){
    this->Cd();
    f_->Close();
  }

}
