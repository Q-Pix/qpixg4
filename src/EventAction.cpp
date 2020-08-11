// -----------------------------------------------------------------------------
//  G4Basic | EventAction.cpp
//
//
//   * Author: Justo Martin-Albo
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#include "EventAction.h"

// Q-Pix includes
#include "AnalysisManager.h"
#include "MCTruthManager.h"

// GEANT4 includes
#include "G4Event.hh"


EventAction::EventAction(): G4UserEventAction()
{}


EventAction::~EventAction()
{}


void EventAction::BeginOfEventAction(const G4Event*)
{}


void EventAction::EndOfEventAction(const G4Event* event)
{
    // get MC truth manager
    MCTruthManager * mc_truth_manager = MCTruthManager::Instance();

    // get analysis manager
    AnalysisManager * analysis_manager = AnalysisManager::Instance();

    // set event number
    analysis_manager->SetEvent(event->GetEventID());

    // add initial MARLEY particles to analysis manager
    for (auto const& particle : mc_truth_manager->GetInitialMARLEYParticles())
    {
        analysis_manager->AddInitialMARLEYParticle(particle);
    }

    // add final MARLEY particles to analysis manager
    for (auto const& particle : mc_truth_manager->GetFinalMARLEYParticles())
    {
        analysis_manager->AddFinalMARLEYParticle(particle);
    }

    // get map of particles from MC truth manager
    auto const MCParticleMap = mc_truth_manager->GetMCParticleMap();

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

