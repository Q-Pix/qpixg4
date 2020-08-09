// -----------------------------------------------------------------------------
//  TrackingAction.h
//
//
//   * Author: Everybody is an author!
//   * Creation date: 4 August 2020
// -----------------------------------------------------------------------------

#ifndef TRACKING_ACTION_H
#define TRACKING_ACTION_H 1

#include "G4UserTrackingAction.hh"

class TrackingAction : public G4UserTrackingAction
{
    public:
        TrackingAction();
        ~TrackingAction();

        void PreUserTrackingAction(const G4Track*);
        void PostUserTrackingAction(const G4Track*);
};

#endif
