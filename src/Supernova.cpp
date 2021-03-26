// -----------------------------------------------------------------------------
//  Supernova.cpp
//
//  Class definition of Supernova
//   * Author: Everybody is an author!
//   * Creation date: 17 September 2020
// -----------------------------------------------------------------------------

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "Randomize.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"

#include "Supernova.h"

#include <math.h> 


//-----------------------------------------------------------------------------
Supernova::Supernova():
N_Ar39_Decays_(0),N_Ar42_Decays_(0),
N_Kr85_Decays_(0),N_Co60_Decays_(0),
N_K40_Decays_(0),N_K42_Decays_(0),
N_Bi214_Decays_(0),N_Pb214_Decays_(0),
N_Po210_Decays_(0),N_Rn222_Decays_(0)
{
    msg_ = new G4GenericMessenger(this, "/Supernova/", "Control commands of the supernova generator.");
    msg_->DeclareProperty("Event_Window", Event_Window_,  "window to simulate the times").SetUnit("ns");

    msg_->DeclareProperty("N_Ar39_Decays", N_Ar39_Decays_,  "number of Ar39 decays");
    msg_->DeclareProperty("N_Ar42_Decays", N_Ar42_Decays_,  "number of Ar42 decays");
    msg_->DeclareProperty("N_Kr85_Decays", N_Kr85_Decays_,  "number of Kr85 decays");
    msg_->DeclareProperty("N_Co60_Decays", N_Co60_Decays_,  "number of Co60 decays");
    msg_->DeclareProperty("N_K40_Decays", N_K40_Decays_,  "number of K40 decays");
    msg_->DeclareProperty("N_K42_Decays", N_K42_Decays_,  "number of K42 decays");
    msg_->DeclareProperty("N_Bi214_Decays", N_Bi214_Decays_,  "number of Bi214 decays");
    msg_->DeclareProperty("N_Pb214_Decays", N_Pb214_Decays_,  "number of Pb214 decays");
    msg_->DeclareProperty("N_Po210_Decays", N_Po210_Decays_,  "number of Po210 decays");
    msg_->DeclareProperty("N_Rn222_Decays", N_Rn222_Decays_,  "number of Rn222 decays");

}

//-----------------------------------------------------------------------------
Supernova::~Supernova()
{
    delete msg_;
}


//-----------------------------------------------------------------------------
void Supernova::Gen_Supernova_Background(G4Event* event)
{
    // Generate Ar39
    for (int ct=0; ct<N_Ar39_Decays_; ct++)
    {
        decay_time = G4UniformRand() * Event_Window_;
        if (G4UniformRand() < 0.5){decay_time *= -1.0;}

        Generate_Radioisotope(event, 18, 39, decay_time, "Vol"); //Ar39 from Volume
    }

    // Generate Ar42
    for (int ct=0; ct<N_Ar42_Decays_; ct++)
    {
        decay_time = G4UniformRand() * Event_Window_;
        if (G4UniformRand() < 0.5){decay_time *= -1.0;}
        Generate_Radioisotope(event, 18, 42, decay_time, "Vol"); //Ar42 from Volume
    }

    // Generate Kr85
    for (int ct=0; ct<N_Kr85_Decays_; ct++)
    {
        decay_time = G4UniformRand() * Event_Window_;
        if (G4UniformRand() < 0.5){decay_time *= -1.0;}

        Generate_Radioisotope(event, 36, 85, decay_time, "Vol"); //Kr85 from Volume
    }

    // Generate Co60
    for (int ct=0; ct<N_Co60_Decays_; ct++)
    {
        decay_time = G4UniformRand() * Event_Window_;
        if (G4UniformRand() < 0.5){decay_time *= -1.0;}
        Generate_Radioisotope(event, 27, 60, decay_time, "CPA"); //Co60 from CPA
    }
    
    // Generate K40
    for (int ct=0; ct<N_K40_Decays_; ct++)
    {
        decay_time = G4UniformRand() * Event_Window_;
        if (G4UniformRand() < 0.5){decay_time *= -1.0;}
        Generate_Radioisotope(event, 19, 40, decay_time, "APA"); //K40 from APA
    }

    // Generate K42
    for (int ct=0; ct<N_K42_Decays_; ct++)
    {
        decay_time = G4UniformRand() * Event_Window_;
        if (G4UniformRand() < 0.5){decay_time *= -1.0;}
        Generate_Radioisotope(event, 19, 42, decay_time, "Vol"); //K42 from Volume
    }

    // Generate Bi214
    for (int ct=0; ct<N_Bi214_Decays_; ct++)
    {
        decay_time = G4UniformRand() * Event_Window_;
        if (G4UniformRand() < 0.5){decay_time *= -1.0;}
        Generate_Radioisotope(event, 83, 214, decay_time, "Vol"); //Bi214 from Volume
    }

    // Generate Pb214
    for (int ct=0; ct<N_Pb214_Decays_; ct++)
    {
        decay_time = G4UniformRand() * Event_Window_;
        if (G4UniformRand() < 0.5){decay_time *= -1.0;}
        Generate_Radioisotope(event, 82, 214, decay_time, "Vol"); //Pb214 from Volume
    }

    // Generate Po210
    for (int ct=0; ct<N_Po210_Decays_; ct++)
    {
        decay_time = G4UniformRand() * Event_Window_;
        if (G4UniformRand() < 0.5){decay_time *= -1.0;}
        Generate_Radioisotope(event, 84, 210, decay_time, "APA"); //Po210 from APA
    }
    
    // Generate Rn222
    for (int ct=0; ct<N_Rn222_Decays_; ct++)
    {
        decay_time = G4UniformRand() * Event_Window_;
        if (G4UniformRand() < 0.5){decay_time *= -1.0;}
        Generate_Radioisotope(event, 86, 222, decay_time, "Vol"); //Rn222 from Volume
    }

}


//-----------------------------------------------------------------------------
void Supernova::Generate_Radioisotope(G4Event* event, int Atomic_Number, int Atomic_Mass, double Decay_Time, std::string Region)
{
    G4ParticleDefinition* pdef = G4IonTable::GetIonTable()->GetIon(Atomic_Number, Atomic_Mass, 0.); // Ar39
    if (!pdef)G4Exception("SetParticleDefinition()", "[IonGun]",FatalException, " can not create ion ");

    // pdef->SetPDGLifeTime(1.*CLHEP::ps);
    pdef->SetPDGLifeTime(1.*ps);

    G4PrimaryParticle* particle = new G4PrimaryParticle(pdef);

    Random_Direction(Px_hat, Py_hat, Pz_hat, 1);
    particle->SetMomentumDirection(G4ThreeVector(Px_hat, Py_hat, Pz_hat));
    particle->SetKineticEnergy(1.*eV); // just an ion sitting

    if (Region == "Vol")
    {
        Gen_Uniform_Position( Ran_X_,  Ran_Y_,  Ran_Z_);
    }
    else if (Region == "APA")
    {
        Gen_APA_Position( Ran_X_,  Ran_Y_,  Ran_Z_);
    }
    else if (Region == "CPA")
    {
        Gen_CPA_Position( Ran_X_,  Ran_Y_,  Ran_Z_);
    }
    else
    {
        G4Exception("invilad ion region", "[supernova]",FatalException, " can not get Region");
    }
    
    G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(Ran_X_,Ran_Y_,Ran_Z_), Decay_Time);
    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);    
}



//-----------------------------------------------------------------------------
void Supernova::Gen_Uniform_Position(double& Ran_X, double& Ran_Y, double& Ran_Z)
{
    Ran_X = G4UniformRand() * detector_length_x_;
    Ran_Y = G4UniformRand() * detector_length_y_;
    Ran_Z = G4UniformRand() * detector_length_z_;
}

//-----------------------------------------------------------------------------
void Supernova::Gen_CPA_Position(double& Ran_X, double& Ran_Y, double& Ran_Z)
{
    Ran_X = G4UniformRand() * detector_length_x_;
    Ran_Y = G4UniformRand() * detector_length_y_;
    Ran_Z = detector_length_z_ - 1 *um;
}

//-----------------------------------------------------------------------------
void Supernova::Gen_APA_Position(double& Ran_X, double& Ran_Y, double& Ran_Z)
{

    double case_weight = G4UniformRand();
    Ran_X = -10 *m;
    Ran_Y = -10 *m;
    Ran_Z = 1 *um;

    if (case_weight <= 0.6)
    {
        int case_number = ceil(G4UniformRand()*3);
        // APA 3x long supports
        if (case_number == 1)
        {
            Ran_X = 0.0 *m  + G4UniformRand() * 0.10 *m;
            Ran_Y = 0.15 *m + G4UniformRand() * (6 *m - 0.15*2 *m);
        }
        else if (case_number == 2 )
        {
            Ran_X = 1.10 *m + G4UniformRand() * 0.10 *m;
            Ran_Y = 0.15 *m + G4UniformRand() * (6 *m - 0.15*2 *m);
        }
        else if (case_number == 3 )
        {
            Ran_X = 2.20 *m + G4UniformRand() * 0.10 *m;
            Ran_Y = 0.15 *m + G4UniformRand() * (6 *m - 0.15*2 *m);
        }
    }
    else if (case_weight >= 0.6 && case_weight <= 0.8)
    {
        int case_number = ceil(G4UniformRand()*2);
        // APA header and footer
        if (case_number == 1 )
        {
            Ran_X = G4UniformRand() * 2.3 *m;
            Ran_Y = G4UniformRand() * 0.15 *m;
        }
        else if (case_number == 2 )
        {
            Ran_X = G4UniformRand() * 2.3 *m;
            Ran_Y = 6 *m - G4UniformRand() * 0.15 *m;
        }
    }
    else if (case_weight >= 0.8)
    {
        int case_number = ceil(G4UniformRand()*8);
        // Now the 8x cross braces
        if (case_number == 1 )
        {
            Ran_X = 0.10 *m + G4UniformRand() * 1.0 *m;
            Ran_Y = 0.15 *m + 1.115*1 *m + G4UniformRand() * (0.05 *m);
        }
        else if (case_number == 2 )
        {
            Ran_X = 0.10 *m + G4UniformRand() * 1.0 *m;
            Ran_Y = 0.15 *m + 1.115*2 *m + G4UniformRand() * (0.05 *m);
        }
        else if (case_number == 3 )
        {
            Ran_X = 0.10 *m + G4UniformRand() * 1.0 *m;
            Ran_Y = 0.15 *m + 1.115*3 *m + G4UniformRand() * (0.05 *m);
        }
        else if (case_number == 4 )
        {
            Ran_X = 0.10 *m + G4UniformRand() * 1.0 *m;
            Ran_Y = 0.15 *m + 1.115*4 *m + G4UniformRand() * (0.05 *m);
        }
        else if (case_number == 5 )
        {
            Ran_X = 1.20 *m + G4UniformRand() * 1.0 *m;
            Ran_Y = 0.15 *m + 1.115*1 *m + G4UniformRand() * (0.05 *m);
        }
        else if (case_number == 6 )
        {
            Ran_X = 1.20 *m + G4UniformRand() * 1.0 *m;
            Ran_Y = 0.15 *m + 1.115*2 *m + G4UniformRand() * (0.05 *m);
        }
        else if (case_number == 7 )
        {
            Ran_X = 1.20 *m + G4UniformRand() * 1.0 *m;
            Ran_Y = 0.15 *m + 1.115*3 *m + G4UniformRand() * (0.05 *m);
        }
        else if (case_number == 8 )
        {
            Ran_X = 1.20 *m + G4UniformRand() * 1.0 *m;
            Ran_Y = 0.15 *m + 1.115*4 *m + G4UniformRand() * (0.05 *m);
        }
    }  
    
}


//-----------------------------------------------------------------------------
inline void Supernova::Random_Direction(double& dx, double& dy, double& dz, const double length = 1.) 
{
    const double phi = 2*M_PI * G4UniformRand();
    const double ctheta = 2 * G4UniformRand() - 1.;
    const double stheta = sqrt(1. - ctheta * ctheta);
    dx = length * cos(phi) * stheta;
    dy = length * sin(phi) * stheta;
    dz = length * ctheta;
}

//-----------------------------------------------------------------------------
void Supernova::Get_Detector_Dimensions(double detector_x_, double detector_y_, double detector_z_)
{
    detector_length_x_ = detector_x_;
    detector_length_y_ = detector_y_;
    detector_length_z_ = detector_z_;
}


