// -----------------------------------------------------------------------------
//  AnalysisManager.h
//
//  Class definition of the analysis manager
//   * Author: Everybody is an author!
//   * Creation date: 4 August 2020
// -----------------------------------------------------------------------------

#ifndef AnalysisManager_h
#define AnalysisManager_h 1

// Qpix includes
#include "AnalysisData.h"

// // GEANT4 includes
// #include </cvmfs/larsoft.opensciencegrid.org/products/geant4/v4_11_1_p01ba/Linux64bit+3.10-2.17-e20-prof/include/Geant4/globals.hh>

// C++ includes
#include <map>
#include <set>

class TFile;
class TTree;
class AnalysisData;

class AnalysisManager {

  public:
    ~AnalysisManager();

    void Book(const std::string&);
    void Save();
    void EventFill(const AnalysisData&);
    void FillMetadata();
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

    std::set< std::string > process_names_;

    // variables that will go into the metadata tree
    double detector_length_x_;
    double detector_length_y_;
    double detector_length_z_;
    bool useHDDetectorConfiguration_;

};

#endif
