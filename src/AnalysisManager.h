// -----------------------------------------------------------------------------
//  AnalysisManager.h
//
//  Class definition of the analysis manager
//   * Author: Everybody is an author!
//   * Creation date: 4 August 2020
// -----------------------------------------------------------------------------

#pragma once
// #ifndef AnalysisManager_h
// #define AnalysisManager_h 1

// Qpix includes
#include "AnalysisData.h"

// GEANT4 includes
#include "globals.hh"

// C++ includes
#include <map>
#include <set>

class TFile;
class TTree;
class AnalysisData;

class AnalysisManager {

  public:
    ~AnalysisManager();

        void Book(std::string const&);
        void Save();
        void Cd();
        void EventFill(const AnalysisData& rhs);
        void EventReset();

        double GetEnergy() const {return energy_deposit_;};
        void SetRun(int const);
        void SetEvent(int const);
        void SetParticleID(int const id){m_particle_id = id;};

        void FillMetadata();
        void FillROOTMeta(Float_t, Float_t, Float_t, Float_t, Float_t, Float_t,
                          Int_t, Int_t, Float_t, Int_t, Int_t, Int_t, Int_t, Int_t, Float_t,
                          Float_t, Float_t, Float_t, Float_t, Float_t, Float_t, Float_t, double);

        void AddMCParticle(MCParticle const *);
        // adding to fill entries in tree by hits instead of events
        void FillMCParticle(MCParticle const *);

        int ProcessToKey(std::string const &);

        inline void AddProcess(std::string const & process) { process_names_.insert(process); }
        inline std::set< std::string > GetProcessNames() const { return process_names_; }

        static AnalysisManager* Instance();
        AnalysisData event;

  private:

    AnalysisManager();

    static AnalysisManager * instance_;

    // ROOT objects
    TFile * tfile_;
    TTree * metadata_;
    TTree * event_tree_;

    void AddInitialGeneratorParticle(GeneratorParticle const *);
    void AddFinalGeneratorParticle(GeneratorParticle const *);


  private:

        int m_particle_id = 0;

        std::set< std::string > process_names_;

        // variables that will go into the metadata tree
        double detector_length_x_;
        double detector_length_y_;
        double detector_length_z_;
        bool useHDDetectorConfiguration_;

        float_t energy_deposit_ = -42.0;
        float_t axis_x_ = -42.0;
        float_t axis_y_ = -42.0;
        float_t axis_z_ = -42.0;
        float_t xpos_ = -1;
        float_t ypos_ = -1;
        float_t zpos_ = -1;
        int nEvt_ = -1;
        int fsPdg_ = 0;
        float_t fsEnergy_ = -1;
        int fsEvt_ = -1;
        int fsFileNo_ = -1;
        int fsFHC_ = -1;
        int fsRun_ = -1;
        int nFS_ = -1;
        float_t lepKE_ = -1;

        // hadronic information for energy recon
        float_t hadTot_;
        float_t hadPip_;
        float_t hadPim_;
        float_t hadPi0_;
        float_t hadP_;
        float_t hadN_;
        float_t hadOther_;
};

// #endif
