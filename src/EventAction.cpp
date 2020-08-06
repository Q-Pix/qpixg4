// -----------------------------------------------------------------------------
//  G4Basic | EventAction.cpp
//
//
//   * Author: Justo Martin-Albo
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#include "EventAction.h"

// Q-Pix includes
#include "AnalysisManager.h"

// GEANT4 includes
#include "G4Event.hh"


EventAction::EventAction(): G4UserEventAction()
{
}


EventAction::~EventAction()
{
}


void EventAction::BeginOfEventAction(const G4Event* event)
{
    // get event number
    AnalysisManager * analysis_manager = AnalysisManager::Instance();
    analysis_manager->SetEvent(event->GetEventID());
}


void EventAction::EndOfEventAction(const G4Event*)
{
    // write event to ROOT file and reset event variables
    AnalysisManager * analysis_manager = AnalysisManager::Instance();
    analysis_manager->EventFill();
    analysis_manager->EventReset();
}
