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
    std::string detector_configuration_;

};

#endif
