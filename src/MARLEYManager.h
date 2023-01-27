// -----------------------------------------------------------------------------
//  MARLEYManager.h
//
//  Class definition of the MARLEY manager
//   * Author: Everybody is an author!
//   * Creation date: 7 August 2020
// -----------------------------------------------------------------------------

#ifndef MARLEYManager_h
#define MARLEYManager_h 1


// Geant4 includes
#include "G4String.hh"

// MARLEY includes
#include "marley/Generator.hh"

class MARLEYManager {

    public:

        MARLEYManager();
        ~MARLEYManager();

        static MARLEYManager * Instance();

        void Initialize();
        marley::Generator & Generator();

    private:

        static MARLEYManager * instance_;

        // MARLEY event generator object
        marley::Generator marley_generator_;


        G4String    marley_json_;
};

#endif

