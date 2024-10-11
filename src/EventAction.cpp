// -----------------------------------------------------------------------------
//  G4_QPIX | EventAction.cpp
//
//
//   * Author: Everybody is an author!
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#include "EventAction.h"

// Q-Pix includes
#include "AnalysisData.h"
#include "AnalysisManager.h"
#include "ConfigManager.h"
#include "MCTruthManager.h"

// GEANT4 includes
#include "G4Event.hh"
#include "G4GenericMessenger.hh"


EventAction::EventAction():
  G4UserEventAction()
{
}


EventAction::~EventAction()
{
}


void EventAction::BeginOfEventAction(const G4Event*)
{
}


void EventAction::EndOfEventAction(const G4Event* g4event)
{
    event_id_offset_ = ConfigManager::GetEventIDOffset();
    energy_threshold_ = ConfigManager::GetEnergyThreshold();

    // get MC truth manager
    MCTruthManager * mc_truth_manager = MCTruthManager::Instance();

    // get map of particles from MC truth manager
    const MCTruthManager::MCParticleMap& particleMap = mc_truth_manager->GetMCParticleMap();


    double energy_deposited = 0.;

    // add particle to analysis manager
    for (auto const& p : particleMap)
    {
        const MCParticle* particle = p.second;
        energy_deposited += particle->EnergyDeposited();
        // std::cout << "Energy deposited by particle PDG (" << particle->PDGCode() << "): " << particle->EnergyDeposited() << std::endl;
    }

    // int mod = event->GetEventID() % 100000;
    // if (mod == 0)
    // {
        // G4cout << "Event " << event->GetEventID() << "..." << G4endl;
        // G4cout << "Energy threshold: " << energy_threshold_ << G4endl;
        // G4cout << "Total energy deposited: " << energy_deposited << G4endl;
    // }

    // don't save event if total energy deposited is below the energy threshold
    if (energy_deposited < energy_threshold_)
    {
        // reset event variables
        event.EventReset();

        // reset event in MC truth manager
        mc_truth_manager->EventReset();

        return;
    }


    // set event number
    event.SetEvent(g4event->GetEventID() + event_id_offset_);

    // add initial generator particles to analysis manager
    for (auto const& particle : mc_truth_manager->GetInitialGeneratorParticles())
    {
        event.AddInitialGeneratorParticle(particle);
    }

    // add initial generator particles to analysis manager
    for (auto const& particle : mc_truth_manager->GetIntermediateGeneratorParticles())
    {
      event.AddIntermediateGeneratorParticle(particle);
    }

    // add final generator particles to analysis manager
    for (auto const& particle : mc_truth_manager->GetFinalGeneratorParticles())
    {
        event.AddFinalGeneratorParticle(particle);
    }

    // add particle to analysis manager
    for (auto const& p : particleMap)
    {
        const MCParticle* particle = p.second;

        event.AddMCParticle(particle);
    }

    // write event to ROOT file and reset event variables
    AnalysisManager * analysisManager = AnalysisManager::Instance();
    analysisManager->EventFill(event);
    event.EventReset();

    // reset event in MC truth manager
    mc_truth_manager->EventReset();
}

