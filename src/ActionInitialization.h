// -----------------------------------------------------------------------------
// //  G4_QPIX | ActionInitialization.h
// //
// //
// //   * Author: Everybody is an author!
// //   * Creation date: 27 Oct 2022
// // -----------------------------------------------------------------------------
//
#ifndef ActionInitialization_h
#define ActionInitialization_h

#include "G4String.hh"
#include "G4VUserActionInitialization.hh"

class EventAction;
class PrimaryGeneration;
class RunAction;
class TrackingAction;
class SteppingAction;


class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    virtual ~ActionInitialization();

  public:
    virtual void Build() const;
    virtual void BuildForMaster() const;

  private:
    mutable PrimaryGeneration* masterPrimaryGeneration;
    mutable EventAction* masterEventAction;
    mutable RunAction* masterRunAction;
};

#endif
