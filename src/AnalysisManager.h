// -----------------------------------------------------------------------------
//  AnalysisManager.h
//
//  Class definition of the analysis manager
//   * Author: Everybody is an author!
//   * Creation date: 4 August 2020
// -----------------------------------------------------------------------------

#ifndef AnalysisManager_h
#define AnalysisManager_h 1

// GEANT4 includes
#include "globals.hh"

// C++ includes
#include <map>
#include <set>

class TFile;
class TTree;

class AnalysisManager {

  public:
    ~AnalysisManager();

    void Book(std::string const);
    void Save();
    void EventFill(const AnalysisData&);
    void FillMetadata(double const &, double const &, double const &);
    static AnalysisManager* Instance();

  private:

    AnalysisManager();

    static AnalysisManager * instance_;

    // AnalysisData objects
    AnalysisData event; 

    // ROOT objects
    TFile * tfile_;
    TTree * metadata_;
    TTree * event_tree_;

    // variables that will go into the metadata tree
    double detector_length_x_;
    double detector_length_y_;
    double detector_length_z_;

};

#endif
