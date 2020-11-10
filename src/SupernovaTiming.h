// -----------------------------------------------------------------------------
//  SupernovaTiming.h
//
//  Class definition of the supernova timing class
//   * Author: Everybody is an author!
//   * Creation date: 25 September 2020
// -----------------------------------------------------------------------------

#ifndef SupernovaTiming_h
#define SupernovaTiming_h 1

// Q-Pix includes

// GEANT4 includes
#include "globals.hh"
#include "G4GenericMessenger.hh"

// ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

// C++ includes
#include <string>

class SupernovaTiming {

    public:

        SupernovaTiming();
        ~SupernovaTiming();

        void Initialize();
        bool Status();
        double Sample(double const);  // sample time [sec] given energy [MeV]

        inline bool Initialized() const { return initialized_; }

    private:

        G4GenericMessenger * msg_;  // messenger for configuration parameters

        // configuration parameters
        bool initialized_;
        bool on_;
        std::string input_file_;
        std::string th2_name_;

        // ROOT objects
        TH1D  * th1_;
        TH2D  * th2_;
        TFile * tfile_;

};

#endif
