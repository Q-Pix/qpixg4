// -----------------------------------------------------------------------------
//  Supernova.h
//
//  Class definition of Supernova
//   * Author: Everybody is an author!
//   * Creation date: 17 September 2020
// -----------------------------------------------------------------------------

#ifndef Supernova_h
#define Supernova_h 1

// #include "G4Box.hh"
// #include "G4String.hh"

class G4GenericMessenger;

class Supernova {

    public:

        Supernova();
        ~Supernova();
        void Gen_Supernova_Background(G4Event*);
        void Gen_test_APA(G4Event*);
        void Get_Detector_Dimensions(double detector_x_, double detector_y_, double detector_z_);

    private:
        G4GenericMessenger* msg_; // Messenger for configuration parameters
        double Event_Window_;
        int N_Ar39_Decays_;
        int N_Ar42_Decays_;
        int N_Kr85_Decays_;
        int N_Co60_Decays_;
        int N_K40_Decays_;
        int N_K42_Decays_;
        int N_Bi214_Decays_;
        int N_Pb214_Decays_;
        int N_Po210_Decays_;
        int N_Rn222_Decays_;

        double decay_time;
        
        double detector_length_x_;
        double detector_length_y_;
        double detector_length_z_;

        double Ran_X_;
        double Ran_Y_;
        double Ran_Z_;

        double Px_hat ;
        double Py_hat ;
        double Pz_hat ;

        inline void Random_Direction(double& dx, double& dy, double& dz, const double length);

        void Gen_APA_Position(double& Ran_X, double& Ran_Y, double& Ran_Z);
        void Gen_CPA_Position(double& Ran_X, double& Ran_Y, double& Ran_Z);
        void Gen_Uniform_Position(double& Ran_X, double& Ran_Y, double& Ran_Z);

        void Generate_Radioisotope(G4Event* event, int Atomic_Number, int Atomic_Mass, double Decay_Time, std::string Region);

};

#endif
