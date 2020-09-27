// -----------------------------------------------------------------------------
//  MARLEYManager.h
//
//  Class definition of the MARLEY manager
//   * Author: Everybody is an author!
//   * Creation date: 7 August 2020
// -----------------------------------------------------------------------------

#ifndef MARLEYManager_h
#define MARLEYManager_h 1

// MARLEY includes
#include "marley/Generator.hh"

class MARLEYManager {

    public:

        MARLEYManager();
        ~MARLEYManager();

        static MARLEYManager * Instance();

        void Initialize(std::string);
        marley::Generator & Generator();

    private:

        static MARLEYManager * instance_;

        // MARLEY event generator object
        marley::Generator marley_generator_;

};

#endif

