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
        void Gen_test_APA(G4Event*);
        void Get_Detector_Dimensions(double detector_x_, double detector_y_, double detector_z_);

    private:
        double detector_length_x_;
        double detector_length_y_;
        double detector_length_z_;

        double Ran_X ;
        double Ran_Y ;
        double Ran_Z ;

        double Px_hat ;
        double Py_hat ;
        double Pz_hat ;

        inline void Random_Direction(double& dx, double& dy, double& dz, const double length);

        void Gen_APA_Position(double& Ran_X, double& Ran_Y, double& Ran_Z);
        void Gen_CPA_Position(double& Ran_X, double& Ran_Y, double& Ran_Z);
        void Gen_Uniform_Position(double& Ran_X, double& Ran_Y, double& Ran_Z);

        

};

#endif
