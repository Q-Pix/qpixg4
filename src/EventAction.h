// -----------------------------------------------------------------------------
//  G4_QPIX | EventAction.h
//
//
//   * Author: Everybody is an author!
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#ifndef EVENT_ACTION_H
#define EVENT_ACTION_H

// QPix includes
#include "AnalysisData.h"

// #include <G4UserEventAction.hh>
#include </cvmfs/larsoft.opensciencegrid.org/products/geant4/v4_11_1_p01ba/Linux64bit+3.10-2.17-e20-prof/include/Geant4/G4UserEventAction.hh>

class G4Event;
class G4GenericMessenger;
class AnalysisData;

class EventAction: public G4UserEventAction
{
  public:

    EventAction();
    virtual ~EventAction();
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    AnalysisData event;

  private:

    G4int event_id_offset_;
    G4double energy_threshold_;
};

#endif
