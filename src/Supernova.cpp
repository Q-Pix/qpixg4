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

#include "Supernova.h"

#include <math.h> 


//-----------------------------------------------------------------------------
Supernova::Supernova()
{}

//-----------------------------------------------------------------------------
Supernova::~Supernova()
{}



void Supernova::Get_Detector_Dimensions(double detector_x_, double detector_y_, double detector_z_)
{
    detector_length_x_ = detector_x_;
    detector_length_y_ = detector_y_;
    detector_length_z_ = detector_z_;
}

void Supernova::Gen_test(G4Event* event)
{

    G4ParticleDefinition* pdef = G4IonTable::GetIonTable()->GetIon(18, 39, 0.); // Ar39
    if (!pdef)G4Exception("SetParticleDefinition()", "[IonGun]",FatalException, " can not create ion ");

    pdef->SetPDGLifeTime(1.*s);

    G4PrimaryParticle* particle = new G4PrimaryParticle(pdef);

    Random_Direction(Px_hat, Py_hat, Pz_hat, 1);
    particle->SetMomentumDirection(G4ThreeVector(Px_hat, Py_hat, Pz_hat));
    particle->SetKineticEnergy(1.*eV); // just an ion sitting

    // Gen_Uniform_Position( Ran_X,  Ran_Y,  Ran_Z);
    // Gen_APA_Position( Ran_X,  Ran_Y,  Ran_Z);
    Gen_CPA_Position( Ran_X,  Ran_Y,  Ran_Z);
    
    G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(Ran_X,Ran_Y,Ran_Z), 0.);
    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);    

}


void Supernova::Gen_Uniform_Position(double& Ran_X, double& Ran_Y, double& Ran_Z)
{
    Ran_X = G4UniformRand() * detector_length_x_;
    Ran_Y = G4UniformRand() * detector_length_y_;
    Ran_Z = G4UniformRand() * detector_length_z_;
}

void Supernova::Gen_CPA_Position(double& Ran_X, double& Ran_Y, double& Ran_Z)
{
    Ran_X = G4UniformRand() * detector_length_x_;
    Ran_Y = G4UniformRand() * detector_length_y_;
    Ran_Z = detector_length_z_ - 1 *um;
}

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
            Ran_Y = 1.115*1 *m + G4UniformRand() * (0.05 *m);
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

inline void Supernova::Random_Direction(double& dx, double& dy, double& dz, const double length = 1.) 
{
    const double phi = 2*M_PI * G4UniformRand();
    const double ctheta = 2 * G4UniformRand() - 1.;
    const double stheta = sqrt(1. - ctheta * ctheta);
    dx = length * cos(phi) * stheta;
    dy = length * sin(phi) * stheta;
    dz = length * ctheta;
}


