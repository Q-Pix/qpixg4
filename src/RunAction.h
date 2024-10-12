// -----------------------------------------------------------------------------
//  G4_QPIX | RunAction.h
//
//
//   * Author: Everybody is an author!
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#ifndef RUN_ACTION_H
#define RUN_ACTION_H

#include "AnalysisData.h"

#include <G4UserRunAction.hh>
#include "G4GenericMessenger.hh"

class G4RunManager;class AnalysisData;

class RunAction: public G4UserRunAction
{
    public:

        RunAction();
        virtual ~RunAction();
        virtual void BeginOfRunAction(const G4Run*);
        virtual void   EndOfRunAction(const G4Run*);

    private:

        G4String root_output_path_;
        AnalysisData event;


        G4String        inputFile_;
        G4String        outputFile_;
        G4String        marleyJson_;
        G4String ReadFrom_Root_;
        G4String        generator_;
        G4String        genieFormat_;
        G4bool          multirun_;
        std::string TreeName_;
        int particle_type_=-1;
        G4RunManager * runManager;
        G4String        particleType_; 
};

#endif
