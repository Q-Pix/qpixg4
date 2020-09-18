// -----------------------------------------------------------------------------
//  Supernova.h
//
//  Class definition of Supernova
//   * Author: Everybody is an author!
//   * Creation date: 17 September 2020
// -----------------------------------------------------------------------------

#ifndef Supernova_h
#define Supernova_h 1

#include "G4Box.hh"

class Supernova {

    public:

        Supernova();
        ~Supernova();
        void Gen_test(G4Event*);

    private:

    G4Box* detector_solid_vol_;

};

#endif
