// -----------------------------------------------------------------------------
//  G4_QPIX | SteppingAction.H
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#ifndef STEPPING_ACTION_H
#define STEPPING_ACTION_H


#include "AnalysisData.h"

#include <G4UserSteppingAction.hh>

#include "AnalysisManager.h"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "globals.hh"

class G4GenericMessenger;

class AnalysisData;

class SteppingAction: public G4UserSteppingAction
{
  public:
    SteppingAction();
    virtual ~SteppingAction();
    virtual void UserSteppingAction(const G4Step*);

  private:

    AnalysisData event;
};

#endif
