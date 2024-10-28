// -----------------------------------------------------------------------------
//  G4_QPIX | DetectorConstruction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "ConfigManager.h"
#include "DetectorConstruction.h"
#include "TrackingSD.h"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4LogicalVolumeStore.hh"

#include "CLHEP/Units/SystemOfUnits.h"


DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(), detector_logic_vol(nullptr)
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // WORLD /////////////////////////////////////////////////

  G4double world_size = std::max({ConfigManager::GetDetectorHeight(),ConfigManager::GetDetectorLength(),ConfigManager::GetDetectorWidth()})*CLHEP::m;
  G4Material* world_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

  G4Box* world_solid_vol =
    new G4Box("world.solid", world_size/2., world_size/2., world_size/2.);

  G4LogicalVolume* world_logic_vol =
    new G4LogicalVolume(world_solid_vol, world_mat, "world.logical");
  world_logic_vol->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4VPhysicalVolume* world_phys_vol =
    new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
                      world_logic_vol, "world.physical", 0, false, 0, true);

  // Detector /////////////////////////////////////////////////     
  if (ConfigManager::GetDetectorConfiguration() == "HD") detector_logic_vol = BuildHDDetector();
  if (ConfigManager::GetDetectorConfiguration() == "VD") detector_logic_vol = BuildVDDetector();
  if (ConfigManager::GetDetectorConfiguration() == "TS") detector_logic_vol = BuildTSDetector();
  
  std::cout << " Detector configuration is: " << ConfigManager::GetDetectorConfiguration() << std::endl;

  G4ThreeVector offset(ConfigManager::GetDetectorWidth()/2., ConfigManager::GetDetectorHeight()/2., ConfigManager::GetDetectorLength()/2.);

  new G4PVPlacement(0, offset,
                    detector_logic_vol, "detector.physical", world_logic_vol, false, 0, true);
  //////////////////////////////////////////////////////////

  return world_phys_vol;
}

G4LogicalVolume* DetectorConstruction::BuildBoxDetector(const G4String& name) {
  G4Material* detector_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr");

  G4Box* detector_solid_vol = new G4Box("detector.solid", 
                                        ConfigManager::GetDetectorWidth() / 2., 
                                        ConfigManager::GetDetectorHeight() / 2., 
                                        ConfigManager::GetDetectorLength() / 2.);

  G4LogicalVolume* logic_vol = new G4LogicalVolume(detector_solid_vol, detector_mat, name);
  return logic_vol;
}

G4LogicalVolume* DetectorConstruction::BuildHDDetector() {
  ConfigManager::SetDetectorWidth(2.3 * CLHEP::m);   // detector_x
  ConfigManager::SetDetectorHeight(6.0 * CLHEP::m);  // detector_y
  ConfigManager::SetDetectorLength(3.6 * CLHEP::m);  // detector_z
  return BuildBoxDetector("HD.detector.logical");
}

G4LogicalVolume* DetectorConstruction::BuildVDDetector() {
  ConfigManager::SetDetectorWidth(20.0 * CLHEP::m);  // detector_x
  ConfigManager::SetDetectorHeight(13.0 * CLHEP::m); // detector_y
  ConfigManager::SetDetectorLength(6.5 * CLHEP::m);  // detector_z
  return BuildBoxDetector("VD.detector.logical");
}

G4LogicalVolume* DetectorConstruction::BuildTSDetector() {
  ConfigManager::SetDetectorWidth(0.04 * CLHEP::m);  // detector_x
  ConfigManager::SetDetectorHeight(0.04 * CLHEP::m); // detector_y
  ConfigManager::SetDetectorLength(0.1 * CLHEP::m);  // detector_z
  return BuildBoxDetector("TS.detector.logical");
}

void DetectorConstruction::ConstructSDandField()
{
  // SENSITIVE DETECTOR ////////////////////////////////////
  TrackingSD* tracking_sd = new TrackingSD("/G4QPIX/TRACKING", "TrackingHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(tracking_sd);

  if (detector_logic_vol) {
    SetSensitiveDetector(detector_logic_vol, tracking_sd);
  } else {
    G4cerr << "Error: detector_logic_vol is null in ConstructSDandField." << G4endl;
  }
  //////////////////////////////////////////////////////////
}