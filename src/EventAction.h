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

#include <G4UserEventAction.hh>


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