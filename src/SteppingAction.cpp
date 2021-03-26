// -----------------------------------------------------------------------------
//  G4_QPIX | SteppingAction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "SteppingAction.h"


#include "AnalysisManager.h"
#include "G4VProcess.hh"


SteppingAction::SteppingAction(): G4UserSteppingAction()
{
}


SteppingAction::~SteppingAction()
{
}


void SteppingAction::UserSteppingAction(const G4Step* step)
{
    AnalysisManager * analysis_manager = AnalysisManager::Instance();

    analysis_manager->AddProcess(step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
}
