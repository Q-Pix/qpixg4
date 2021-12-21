//
// Created by ilker on 12/20/21.
//

#include "ROOTManager.h"
#include "iostream"
ROOTManager * ROOTManager::instance_=0;

//--------------------------------
ROOTManager::ROOTManager() {}
//--------------------------------
ROOTManager::~ROOTManager() {
}
//--------------------------------
ROOTManager * ROOTManager::Instance()
{
    if (instance_ == 0) instance_ = new ROOTManager();
    return instance_;
}
int ROOTManager::Initialize(std::string FilePath,std::string TreeName) {
    if (!FilePath.empty())
    {
        FilePath_=FilePath;
        TreeName_=TreeName;
        std::cout << "Configuring to Read From Root File..." << std::endl;
        // Reading the root file to extract the info
        if (tree_ == 0) {
            f_ = (TFile*)gROOT->GetListOfFiles()->FindObject(FilePath_.c_str());
            if (!f_ || !f_->IsOpen()) {
                f_ = new TFile(FilePath.c_str());
            }
            f_->GetObject(TreeName_.c_str(),tree_);

        }
        return 1;
    }
    else {
        std::cout << "Generating from Root is not selected!  Continuing..."<< std::endl;
        return 0;
    }


}


void ROOTManager::SetBranches() {
    // Creating Root Branches

    tree_->SetBranchAddress("event", &event_, &b_event);
    tree_->SetBranchAddress("NParticles", &NParticles_, &b_NParticles);
    tree_->SetBranchAddress("pdg", pdg_, &b_pdg);
    tree_->SetBranchAddress("E", E_, &b_E);
    tree_->SetBranchAddress("px", px_, &b_px);
    tree_->SetBranchAddress("py", py_, &b_py);
    tree_->SetBranchAddress("pz", pz_, &b_pz);

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