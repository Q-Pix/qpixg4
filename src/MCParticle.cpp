// -----------------------------------------------------------------------------
//  MCParticle.cpp
//
//  Class definition of MCParticle
//   * Author: Everybody is an author!
//   * Creation date: 7 August 2020
// -----------------------------------------------------------------------------

#include "MCParticle.h"

// GEANT4 includes
#include "G4VProcess.hh"

//-----------------------------------------------------------------------------
MCParticle::MCParticle()
{}

//-----------------------------------------------------------------------------
MCParticle::~MCParticle()
{}

//-----------------------------------------------------------------------------
// void MCParticle::AddTrajectoryPoint(const TrajectoryPoint & point)
// {
//     trajectory_.push_back(point);
// }

//-----------------------------------------------------------------------------
void MCParticle::AddTrajectoryHit(const TrajectoryHit & hit)
{
    hits_.push_back(hit);
}

//-----------------------------------------------------------------------------
void MCParticle::AddTrajectoryHit(const G4Step * step)
{
    // initialize hit
    TrajectoryHit hit;
    hit.start_ = { step->GetPreStepPoint()->GetPosition().x()/CLHEP::cm,
                   step->GetPreStepPoint()->GetPosition().y()/CLHEP::cm,
                   step->GetPreStepPoint()->GetPosition().z()/CLHEP::cm, };
    hit.end_   = { step->GetPostStepPoint()->GetPosition().x()/CLHEP::cm,
                   step->GetPostStepPoint()->GetPosition().y()/CLHEP::cm,
                   step->GetPostStepPoint()->GetPosition().z()/CLHEP::cm, };
    hit.energy_deposit_ = step->GetTotalEnergyDeposit() / CLHEP::MeV;
    hit.start_time_ = step->GetPreStepPoint()->GetGlobalTime() / CLHEP::ns;
    hit.end_time_   = step->GetPostStepPoint()->GetGlobalTime() / CLHEP::ns;
    hit.track_id_   = step->GetTrack()->GetTrackID();
    hit.pdg_code_   = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
    hit.length_     = step->GetStepLength() / CLHEP::cm;
    hit.process_    = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

    energy_deposited_ += step->GetTotalEnergyDeposit() / CLHEP::MeV;

    // add hit
    this->AddTrajectoryHit(hit);
}

//-----------------------------------------------------------------------------
void MCParticle::AddDaughter(int const trackID)
{
    // add daughter track ID and increment counter
    daughter_track_ids_.push_back(trackID);
    number_daughters_++;
}

