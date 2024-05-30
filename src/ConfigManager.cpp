// -----------------------------------------------------------------------------
//  ConfigManager.cpp
//
//  Class definition of the ConfigManager Singleton Container
//   * Author: Everybody is an author!
//   * Creation date: 18 November 2022
// -----------------------------------------------------------------------------

// Class Includes
#include "ConfigManager.h"

// Geant Includes
#include "globals.hh"
#include "G4GenericMessenger.hh"
#include "G4Threading.hh"
#include "G4ThreeVector.hh"
#include "CLHEP/Units/SystemOfUnits.h"

// System Includes
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <float.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Singleton Initializers for master and worker threads
ConfigManager* ConfigManager::Instance() {
  static const ConfigManager* masterInstance = 0;
  static G4ThreadLocal ConfigManager* theInstance = 0;

  if (!theInstance) {
    if (!G4Threading::IsWorkerThread()) {   // Master or sequential
      theInstance = new ConfigManager;
      masterInstance = theInstance;
    } else {                    // Workers copy from master
      theInstance = new ConfigManager(*masterInstance);
    }
  }

  return theInstance;
}

//-----------------------------------------------------------------------------
ConfigManager::ConfigManager()
  : eventIDOffset_(0), energyThreshold_(0),
  particleType_(""), decayAtTimeZero_(false), isotropic_(true),
  overrideVertexPosition_(false), printParticleInfo_(false), inputFile_(""), outputFile_(""), marleyJson_(""), generator_(""),
  genieFormat_(""), multirun_(false), momentumDirection_(0,0,0), vertexX_(2.3*CLHEP::m/2), vertexY_(6.0*CLHEP::m/2), vertexZ_(3.7*CLHEP::m/2),
  nAr39Decays_(0), nAr42Decays_(0), nKr85Decays_(0), nCo60Decays_(0), nK40Decays_(0),
  nK42Decays_(0), nBi214Decays_(0), nPb214Decays_(0), nPo210Decays_(0), nRn222Decays_(0), eventCutoff_(0),
  eventWindow_(0),
  snTimingOn_(false), th2Name_("nusperbin2d_nue"),
  useHDDetectorConfiguration_(true), detectorLength_(0), detectorWidth_(0), detectorHeight_(0), 
  worldLength_(0), worldWidth_(0), worldHeight_(0), getPhotons_(false), getElectrons_(false), nbverbose_(0)
{
  CreateCommands();
}

//-----------------------------------------------------------------------------
ConfigManager::ConfigManager(const ConfigManager& master)
  : eventIDOffset_(master.eventIDOffset_),
  energyThreshold_(master.energyThreshold_),
  particleType_(master.particleType_),
  decayAtTimeZero_(master.decayAtTimeZero_), isotropic_(master.isotropic_),
  overrideVertexPosition_(master.overrideVertexPosition_),
  printParticleInfo_(master.printParticleInfo_), inputFile_(master.inputFile_),
  outputFile_(master.outputFile_), marleyJson_(master.marleyJson_),
  generator_(master.generator_), genieFormat_(master.genieFormat_),
  multirun_(master.multirun_), momentumDirection_(master.momentumDirection_),
  vertexX_(master.vertexX_), vertexY_(master.vertexY_),
  vertexZ_(master.vertexZ_),
  nAr39Decays_(master.nAr39Decays_), nAr42Decays_(master.nAr42Decays_),
  nKr85Decays_(master.nKr85Decays_), nCo60Decays_(master.nCo60Decays_),
  nK40Decays_(master.nK40Decays_), nK42Decays_(master.nK42Decays_),
  nBi214Decays_(master.nBi214Decays_), nPb214Decays_(master.nPb214Decays_),
  nPo210Decays_(master.nPo210Decays_), nRn222Decays_(master.nRn222Decays_),
  eventCutoff_(master.eventCutoff_), eventWindow_(master.eventWindow_),
  snTimingOn_(master.snTimingOn_), th2Name_(master.th2Name_),
  useHDDetectorConfiguration_(master.useHDDetectorConfiguration_), detectorLength_(master.detectorLength_),
  detectorWidth_(master.detectorWidth_), detectorHeight_(master.detectorHeight_), worldLength_(master.worldLength_), 
  worldWidth_(master.worldWidth_), worldHeight_(master.worldHeight_), getPhotons_(master.getPhotons_),
  getElectrons_(master.getElectrons_), nbverbose_(master.nbverbose_)
{
  CreateCommands();
}

//-----------------------------------------------------------------------------
ConfigManager::~ConfigManager()
{
  delete msgEvent_;
  delete msgInputs_;
  delete msgSupernova_;
  delete msgSupernovaTiming_;
  delete msgGeometry_;
  delete msgNeutron_;
}

//-----------------------------------------------------------------------------
void ConfigManager::CreateCommands()
{
  // Instantiate all messengers
  msgEvent_ = new G4GenericMessenger(this, "/event/", "user-defined event configuration");
  msgInputs_ = new G4GenericMessenger(this, "/inputs/", "Control commands of the ion primary generator.");
  msgSupernova_ = new G4GenericMessenger(this, "/supernova/", "Control commands of the supernova generator.");
  msgSupernovaTiming_ = new G4GenericMessenger(this, "/supernova/timing/", "control commands for SupernovaTiming");
  msgGeometry_ = new G4GenericMessenger(this, "/geometry/", "control commands for DetectorConstruction");
  msgNeutron_ = new G4GenericMessenger(this, "/neutron/", "control commands for NeutronBackground");

  // Declare all properties for msgEvent
  msgEvent_->DeclareProperty("offset", eventIDOffset_, "Event ID offset.");
  msgEvent_->DeclarePropertyWithUnit("energy_threshold", "MeV", energyThreshold_, "Events that deposit less energy than this energy threshold will not be saved.");

  // Declare all properties for msgInputs
  msgInputs_->DeclareProperty("particle_type", particleType_,  "which kind of particle?");
  msgInputs_->DeclareProperty("decay_at_time_zero", decayAtTimeZero_, "Set to true to make unstable isotopes decay at t=0.");
  msgInputs_->DeclareProperty("isotropic", isotropic_, "isotropic");
  msgInputs_->DeclareProperty("override_vertex_position", overrideVertexPosition_, "override vertex position");
  msgInputs_->DeclareProperty("print_particle_info", printParticleInfo_, "Extra Printing for Debugging");
  msgInputs_->DeclareProperty("input_file", inputFile_, "input ROOT file");
  msgInputs_->DeclareProperty("output_file", outputFile_, "output ROOT file");
  msgInputs_->DeclareProperty("MARLEY_json", marleyJson_, "marley config json file");
  msgInputs_->DeclareProperty("generator", generator_, "event generator of input file");
  msgInputs_->DeclareProperty("multirun", multirun_, "multiple runs");
  msgInputs_->DeclareProperty("genie_format", genieFormat_, "format of genie-produced input file");
  msgInputs_->DeclareProperty("momentum_direction", momentumDirection_, "initial momentum of generator particles");

  msgInputs_->DeclarePropertyWithUnit("vertex_x", "mm", vertexX_, "vertex x");
  msgInputs_->DeclarePropertyWithUnit("vertex_y", "mm", vertexY_, "vertex y");
  msgInputs_->DeclarePropertyWithUnit("vertex_z", "mm", vertexZ_, "vertex z");

  // Declare all properties for msgSupernova
  msgSupernova_->DeclareProperty("N_Ar39_Decays", nAr39Decays_,  "number of Ar39 decays");
  msgSupernova_->DeclareProperty("N_Ar42_Decays", nAr42Decays_,  "number of Ar42 decays");
  msgSupernova_->DeclareProperty("N_Kr85_Decays", nKr85Decays_,  "number of Kr85 decays");
  msgSupernova_->DeclareProperty("N_Co60_Decays", nCo60Decays_,  "number of Co60 decays");
  msgSupernova_->DeclareProperty("N_K40_Decays", nK40Decays_,  "number of K40 decays");
  msgSupernova_->DeclareProperty("N_K42_Decays", nK42Decays_,  "number of K42 decays");
  msgSupernova_->DeclareProperty("N_Bi214_Decays", nBi214Decays_,  "number of Bi214 decays");
  msgSupernova_->DeclareProperty("N_Pb214_Decays", nPb214Decays_,  "number of Pb214 decays");
  msgSupernova_->DeclareProperty("N_Po210_Decays", nPo210Decays_,  "number of Po210 decays");
  msgSupernova_->DeclareProperty("N_Rn222_Decays", nRn222Decays_,  "number of Rn222 decays");

  msgSupernova_->DeclarePropertyWithUnit("Event_Cutoff", "ns", eventCutoff_,  "window to simulate the times");
  msgSupernova_->DeclarePropertyWithUnit("Event_Window", "ns", eventWindow_,  "window to simulate the times");

  // Declare all properties for msgSupernovaTiming
  msgSupernovaTiming_->DeclareProperty("on", snTimingOn_, "turn on SupernovaTiming");
  msgSupernovaTiming_->DeclareProperty("th2_name", th2Name_, "name of TH2");

  // Declare all properties for msgGeometry
  // Detector
  msgGeometry_->DeclareProperty("use_hd_detector_configuration", useHDDetectorConfiguration_, "True if HD, false if VD");
  msgGeometry_->DeclareProperty("detector_length", detectorLength_, "detector length");
  msgGeometry_->DeclareProperty("detector_width", detectorWidth_, "detector width");
  msgGeometry_->DeclareProperty("detector_height", detectorHeight_, "detector height");

  // World
  msgGeometry_->DeclareProperty("world_length", worldLength_, "world length");
  msgGeometry_->DeclareProperty("world_width", worldWidth_, "world width");
  msgGeometry_->DeclareProperty("world_height", worldHeight_, "world height");

  // Declare all properties for msgNeutron
  msgNeutron_->DeclareProperty("get_photons", getPhotons_, "get photons from neutron capture in SteppingAction");
  msgNeutron_->DeclareProperty("get_electrons", getElectrons_, "get secondary electrons from neutron capture in SteppingAction");
  msgNeutron_->DeclareProperty("nb_verbose", nbverbose_, "activate verbose on generation of photons from neutron capture");
}

//-----------------------------------------------------------------------------
void ConfigManager::PrintConfig() const
{
  G4cout << "EventAction -- Event_ID_Offset:    " << eventIDOffset_ << G4endl
     << "EventAction -- Energy_Threshold:   " << energyThreshold_ << G4endl
     << G4endl
     << "Input -- Generator:                " << generator_ << G4endl
     << "Input -- Genie Format:             " << genieFormat_ << G4endl
     << "Input -- Particle_Type:            " << particleType_ << G4endl
     << "Input -- Decay_At_Time_Zero:       " << decayAtTimeZero_ << G4endl
     << "Input -- Isotropic:                " << isotropic_ << G4endl
     << "Input -- Override_Vertex_Position: " << overrideVertexPosition_ << G4endl
     << "Input -- Print_Particle_Info:      " << printParticleInfo_ << G4endl
     << "Input -- Input_File:               " << inputFile_ << G4endl
     << "Input -- Output_File:              " << outputFile_ << G4endl
     << "Input -- MARLEY_json:              " << marleyJson_ << G4endl
     << "Input -- Momentum_Direction:       " << momentumDirection_ << G4endl
     << "Input -- Vertex_X:                 " << vertexX_/CLHEP::mm << " mm" << G4endl
     << "Input -- Vertex_Y:                 " << vertexY_/CLHEP::mm << " mm" <<  G4endl
     << "Input -- Vertex_Z:                 " << vertexZ_/CLHEP::mm << " mm" << G4endl
     << "Input -- Multirun:                 " << multirun_ << G4endl
     << G4endl
     << "Supernova -- N_Ar39_Decays:  " << nAr39Decays_ << G4endl
     << "Supernova -- N_Ar42_Decays:  " << nAr42Decays_ << G4endl
     << "Supernova -- N_Kr85_Decays:  " << nKr85Decays_ << G4endl
     << "Supernova -- N_Co60_Decays:  " << nCo60Decays_ << G4endl
     << "Supernova -- N_K40_Decays:   " << nK40Decays_ << G4endl
     << "Supernova -- N_K42_Decays:   " << nK42Decays_ << G4endl
     << "Supernova -- N_Bi214_Decays: " << nBi214Decays_ << G4endl
     << "Supernova -- N_Pb214_Decays: " << nPb214Decays_ << G4endl
     << "Supernova -- N_Po210_Decays: " << nPo210Decays_ << G4endl
     << "Supernova -- N_Rn222_Decays: " << nRn222Decays_ << G4endl
     << "Supernova -- Event_Cutoff:   " << eventCutoff_/CLHEP::ns << " ns" << G4endl
     << "Supernova -- Event_Window:   " << eventWindow_/CLHEP::ns << " ns" << G4endl
     << G4endl
     << "SupernovaTiming -- Supernova_Timing_On: " << snTimingOn_ << G4endl
     << "SupernovaTiming -- TH2_Name:            " << th2Name_ << G4endl
     << G4endl
     << "Geometry -- Use_HD_Detector_Configuration: " << useHDDetectorConfiguration_ << G4endl
     << "Geometry -- Detector_Length:               " << detectorLength_/CLHEP::m << " m" << G4endl
     << "Geometry -- Detector_Width:                " << detectorWidth_/CLHEP::m << " m" << G4endl
     << "Geometry -- Detector_Height:               " << detectorHeight_/CLHEP::m << " m" <<G4endl
     << "Geometry -- World_Length:                  " << worldLength_/CLHEP::m << " m" << G4endl
     << "Geometry -- World_Width:                   " << worldWidth_/CLHEP::m << " m" << G4endl
     << "Geometry -- World_Height:                  " << worldHeight_/CLHEP::m << " m" <<G4endl
     << G4endl
     << "Neutron -- Use SteppingAction to get photons from neutron capture: " << getPhotons_ << G4endl
     << "Neutron -- Use SteppingAction to get electrons from neutron capture: " << getElectrons_ << G4endl
     << "Neutron -- Activate verbose on generation of photons from neutron capture: " << nbverbose_ << G4endl
     << G4endl;
}