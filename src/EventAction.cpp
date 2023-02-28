// -----------------------------------------------------------------------------
//  G4_QPIX | EventAction.cpp
//
//
//   * Author: Everybody is an author!
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#include "EventAction.h"

// Q-Pix includes
#include "AnalysisManager.h"
#include "MCTruthManager.h"

// GEANT4 includes
#include "G4Event.hh"
#include "G4GenericMessenger.hh"


EventAction::EventAction():
  G4UserEventAction(), event_id_offset_(0), energy_threshold_(0.)
{
    msg_ = new G4GenericMessenger(this, "/event/", "user-defined event configuration");
    msg_->DeclareProperty("offset", event_id_offset_, "Event ID offset.");
    msg_->DeclareProperty("energy_threshold", energy_threshold_, "Events that deposit less energy than this energy threshold will not be saved.").SetUnit("MeV");
}


EventAction::~EventAction()
{
}


void EventAction::BeginOfEventAction(const G4Event*)
{
    // int mod = event->GetEventID() % 1000;
    // if (mod == 0)
    // {
    //     G4cout << "Starting event" << event->GetEventID() << "..." << G4endl;
    // }
}


void EventAction::EndOfEventAction(const G4Event* event)
{
    // get MC truth manager
    MCTruthManager * mc_truth_manager = MCTruthManager::Instance();

    // get map of particles from MC truth manager
    auto const MCParticleMap = mc_truth_manager->GetMCParticleMap();

    double energy_deposited = 0.;

    // add particle to analysis manager
    for (auto const& p : MCParticleMap)
    {
        auto const& particle = p.second;
        energy_deposited += particle->EnergyDeposited();
        // std::cout << "Energy deposited by particle PDG (" << particle->PDGCode() << "): " << particle->EnergyDeposited() << std::endl;
    }

    int mod = event->GetEventID() % 1000;
    if (mod == 0)
    {
        G4cout << "Event " << event->GetEventID() << "..." << G4endl;
        // G4cout << "Energy threshold: " << energy_threshold_ << G4endl;
        // G4cout << "Total energy deposited: " << energy_deposited << G4endl;
    }

    // don't save event if total energy deposited is below the energy threshold
    if (energy_deposited < energy_threshold_)
    {
        // get analysis manager
        AnalysisManager * analysis_manager = AnalysisManager::Instance();

        // reset event variables
        analysis_manager->EventReset();

        // reset event in MC truth manager
        mc_truth_manager->EventReset();

        return;
    }

    // get analysis manager
    AnalysisManager * analysis_manager = AnalysisManager::Instance();

    // set event number
    // event->SetEventID(event->GetEventID() + event_id_offset_);
    // analysis_manager->SetEvent(event->GetEventID());
    analysis_manager->SetEvent(event->GetEventID() + event_id_offset_);

    // add initial generator particles to analysis manager
    // for (auto const& particle : mc_truth_manager->GetInitialGeneratorParticles())
    // {
        // analysis_manager->AddInitialGeneratorParticle(particle);
    // }

    // add final generator particles to analysis manager
    // for (auto const& particle : mc_truth_manager->GetFinalGeneratorParticles())
    // {
        // analysis_manager->AddFinalGeneratorParticle(particle);
    // }

    // get map of particles from MC truth manager
    // auto const MCParticleMap = mc_truth_manager->GetMCParticleMap();

    // add particle to analysis manager
    for (auto const& p : MCParticleMap)
    {
        auto const& particle = p.second;

        analysis_manager->AddMCParticle(particle);
    }

    // write event to ROOT file and reset event variables
    analysis_manager->EventFill();
    analysis_manager->EventReset();

    // reset event in MC truth manager
    mc_truth_manager->EventReset();
}

