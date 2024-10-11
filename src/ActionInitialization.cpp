// -----------------------------------------------------------------------------
// G4_QPIX | QPixActionInitialization.cpp
//
//
//   * Author: Dave Elofson (dave.elofson@tamu.edu)
//   * Creation date: 27 Oct 2022
// -----------------------------------------------------------------------------

// Class Includes
#include "ActionInitialization.h"

// Project Includes
#include "EventAction.h"
#include "PrimaryGeneration.h"
#include "RunAction.h"
#include "SteppingAction.h"
#include "TrackingAction.h"

// Geant4 Includes
#include "G4String.hh"

// System Includes

// Constructor and Destructor
ActionInitialization::ActionInitialization()
  : masterPrimaryGeneration(0), masterEventAction(0)
{
}

ActionInitialization::~ActionInitialization()
{
  delete masterPrimaryGeneration;    masterPrimaryGeneration=0;
  delete masterEventAction;          masterEventAction=0;
}

// Create RunAction for master thread only

void ActionInitialization::BuildForMaster() const
{

  SetUserAction(new RunAction());

  // To get UI commands prior to /run/initialize, create local actions here (not registered)
  masterPrimaryGeneration = new PrimaryGeneration;
  masterEventAction = new EventAction;
}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneration);
  SetUserAction(new RunAction());
  SetUserAction(new EventAction);
  SetUserAction(new TrackingAction);
  SetUserAction(new SteppingAction);
}


