//
// Created by ilker on 12/20/21.
//



#ifndef QPIXG4_ROOTMANAGER_H
#define QPIXG4_ROOTMANAGER_H
// ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

// C++ includes
#include <map>
#include <set>
class ROOTManager {

    public:

        ROOTManager();
        ~ROOTManager();

        static ROOTManager * Instance();
        int Initialize(std::string FilePath,std::string TreeName);
        static const Int_t   Size_=51;

        Int_t Getevent_ () const{return event_; };
        Int_t GetNParticles_ () const{return NParticles_; };
        TFile* GetFile_ () const{return f_; };
        TTree* GetTTree_ () const{return tree_; };
        Int_t  GetPDG_(Int_t ev) const {return pdg_[ev];};
        Int_t  GetIdx_(Int_t ev) const {return idx[ev];};
        Int_t  GetFirstMother_(Int_t ev) const {return FirstMother[ev];};
        Int_t  IsROOTManagerInitialized_() const {return isInitialized;};


        Float_t  GetE_(Int_t ev) const {return E_[ev];};
        Float_t  GetPx_(Int_t ev) const {return px_[ev];};
        Float_t  GetPy_(Int_t ev) const {return py_[ev];};
        Float_t  GetPz_(Int_t ev) const {return pz_[ev];};
        std::string GetInPutRootFilePath()const {return FilePath_;};
        std::string  GetInPutRootTreeName()const{return TreeName_;};


    void SetBranches();
        Int_t GetNEntries();
        void Cd();
        void Close();


    protected:

        Int_t           event_;
        Int_t           NParticles_;
        TFile *         f_=0;
        TTree *         tree_=0;
        Int_t           pdg_[Size_];
        Float_t        E_[Size_];
        Float_t        px_[Size_];
        Float_t        py_[Size_];
        Float_t        pz_[Size_];
        Int_t           idx[Size_];
        Int_t           Status[Size_];
        Int_t           FirstMother[Size_];
        Int_t           LastMother[Size_];
        Int_t           FirstDaughter[Size_];
        Int_t           LastDaughter[Size_];
        TBranch        *b_event;   //!
        TBranch        *b_NParticles;   //!
        TBranch        *b_pdg;   //!
        TBranch        *b_E;   //!
        TBranch        *b_px;   //!
        TBranch        *b_py;   //!
        TBranch        *b_pz;   //!
        TBranch        *b_Mother;   //!
        TBranch        *b_Daughter;   //!
        TBranch        *bridx;
        TBranch        *brStatus;
        TBranch        *brFirstMother;
        TBranch        *brLastMother;
        TBranch        *brFirstDaughter;
        TBranch        *brLastDaughter;
        std::string     FilePath_;
        std::string     TreeName_;
        Int_t isInitialized;






private:
        static ROOTManager *instance_;



};


#endif //QPIXG4_ROOTMANAGER_H
