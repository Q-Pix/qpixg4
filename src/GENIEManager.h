///////////////////////////////////////////////////////////////////////////////
//
// GENIEManager.h
//
// Created by: Ilker 20122021
// Modifield by: Dave Elofson 01092023
//
///////////////////////////////////////////////////////////////////////////////



#ifndef QPIXG4_ROOTMANAGER_H
#define QPIXG4_ROOTMANAGER_H
// ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

// Geant4 includes
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4Types.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"


// C++ includes
#include <map>
#include <set>
class GENIEManager {

  public:

    ~GENIEManager();

    static GENIEManager * Instance();
    G4int  Initialize();
    static const Int_t   Size_=100;

    G4int  Getevent_()                   const {return event_; };
    G4int  GetNParticles_()              const {return NParticles_; };
    TFile* GetFile_()                    const {return f_; };
    TTree* GetTTree_()                   const {return tree_; };
    G4int  GetPDG_(Int_t ev)             const {return pdg_[ev];};
    G4int  GetIdx_(Int_t ev)             const {return idx[ev];};
    G4int  GetFirstMother_(Int_t ev)     const {return FirstMother[ev];};
    G4int  IsGENIEManagerInitialized_()   const {return isInitialized;};
    G4int  GetStatus_(Int_t value)       const {return Status[value]; };

    G4double  GetE_(Int_t ev)               const {return P4_[ev][3]*CLHEP::GeV;};
    G4double  GetPx_(Int_t ev)              const {return P4_[ev][0]*CLHEP::GeV;};
    G4double  GetPy_(Int_t ev)              const {return P4_[ev][1]*CLHEP::GeV;};
    G4double  GetPz_(Int_t ev)              const {return P4_[ev][2]*CLHEP::GeV;};
    G4String  GetInPutRootFilePath()        const {return inputFile_;};
    G4String  GetInPutRootTreeName()        const {return genieFormat_;};


    void SetBranches();
    Int_t GetNEntries();
    void Cd();
    void Close();
    void LoadEvent(G4Event*);

  protected:

    G4int           event_;
    G4int           NParticles_;
    TFile *         f_=0;
    TTree *         tree_=0;
    G4int           pdg_[Size_];
    G4double        xsec_;
    G4double        dxsec_;
    G4double        weight_;
    G4double        prob_;
    G4double        vtx_[Size_];
    G4double        X4_[Size_][4];
    G4double        P4_[Size_][4];
    G4double        polz_[Size_][3];
    G4int           NParticles;
    G4int           idx[Size_];
    G4int           Status[Size_];
    G4int           rescat_[Size_];
    G4int           FirstMother[Size_];
    G4int           LastMother[Size_];
    G4int           FirstDaughter[Size_];
    G4int           LastDaughter[Size_];
    TBranch*        b_event;   //!
    //TBranch*        b_flags;
    //TBranch*        b_code;
    TBranch*        b_xsec;
    TBranch*        b_dxsec;
    TBranch*        b_weight;
    TBranch*        b_prob;
    TBranch*        b_vtx;
    TBranch*        b_NParticles;   //!
    TBranch*        b_pdg;   //!
    TBranch*        b_rescat;   //!
    TBranch*        b_X4;   //!
    TBranch*        b_P4;   //!
    TBranch*        b_polz;   //!
    TBranch*        b_Mother;   //!
    TBranch*        b_Daughter;   //!
    TBranch*        bridx;
    TBranch*        brStatus;
    TBranch*        brFirstMother;
    TBranch*        brLastMother;
    TBranch*        brFirstDaughter;
    TBranch*        brLastDaughter;
    G4String        inputFile_;
    G4String        particleType_;
    G4String        genieFormat_;
    G4String        treeName_;
    G4int           isInitialized;






  private:

    GENIEManager();

    G4ThreadLocal static GENIEManager *instance_;

  protected:

    // GEANT4 dictionary of particles
    G4ParticleTable* particle_table_;


};


#endif //QPIXG4_ROOTMANAGER_H
