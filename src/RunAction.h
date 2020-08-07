// -----------------------------------------------------------------------------
//  G4Basic | RunAction.h
//
//
//   * Author: Justo Martin-Albo
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#ifndef RUN_ACTION_H
#define RUN_ACTION_H

#include <G4UserRunAction.hh>
#include "G4GenericMessenger.hh"


class RunAction: public G4UserRunAction
{
    public:

        RunAction();
        virtual ~RunAction();
        virtual void BeginOfRunAction(const G4Run*);
        virtual void   EndOfRunAction(const G4Run*);

    private:

        G4GenericMessenger * messenger_;
        G4String root_output_path_;
        G4String marley_json_;
};

#endif
