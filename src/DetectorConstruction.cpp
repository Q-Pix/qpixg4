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


DetectorConstruction::DetectorConstruction(): G4VUserDetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get Detector Geometry first to dicatate world_size
  if(ConfigManager::GetUseHDDetectorConfiguration())
  {
    // DETECTOR HD CONFIGURATION //////////////////////////////////////////////
    // resemble an APA size
    ConfigManager::SetDetectorWidth(2.3*CLHEP::m);   // detector_x
    ConfigManager::SetDetectorHeight(6.0*CLHEP::m);  // detector_y
    ConfigManager::SetDetectorLength(3.6*CLHEP::m);  // detector_z
  } else {
    // DETECTOR VD CONFIGURATION //////////////////////////////////////////////
    ConfigManager::SetDetectorHeight(13.0*CLHEP::m); // detector_y
    ConfigManager::SetDetectorLength(6.5*CLHEP::m);  // detector_z
    ConfigManager::SetDetectorWidth(20.0*CLHEP::m);  // detector_x
  }

  // WORLD /////////////////////////////////////////////////
  G4double world_size = std::max({ConfigManager::GetDetectorHeight(),ConfigManager::GetDetectorLength(),ConfigManager::GetDetectorWidth()})*CLHEP::m;
  
  G4double world_height = ConfigManager::GetWorldHeight()*CLHEP::m; // world_y
  G4double world_length = ConfigManager::GetWorldLength()*CLHEP::m; // world_z
  G4double world_width  = ConfigManager::GetWorldWidth()*CLHEP::m;  // world_x

  if(world_height < ConfigManager::GetDetectorHeight() || 
     world_length < ConfigManager::GetDetectorLength() ||
     world_width  < ConfigManager::GetDetectorWidth())
    {
     world_height = world_size/CLHEP::m;
     world_width  = world_size/CLHEP::m;
     world_length = world_size/CLHEP::m;
    }
  
  G4Material* world_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

  G4Box* world_solid_vol =
    new G4Box("world.solid", world_width/2., world_height/2., world_length/2.);

  G4LogicalVolume* world_logic_vol =
    new G4LogicalVolume(world_solid_vol, world_mat, "world.logical");
  // world_logic_vol->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4VPhysicalVolume* world_phys_vol =
    new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
                      world_logic_vol, "world.physical", 0, false, 0, true);
                      

  std::cout << " Detector configuration is: " << ConfigManager::GetUseHDDetectorConfiguration() << std::endl;

  // DETECTOR
  G4Material* detector_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr");

  G4Box* detector_solid_vol =
    new G4Box("detector.solid", ConfigManager::GetDetectorWidth()/2., ConfigManager::GetDetectorHeight()/2., ConfigManager::GetDetectorLength()/2.);

  G4LogicalVolume* detector_logic_vol =
    new G4LogicalVolume(detector_solid_vol, detector_mat, "detector.logical");

  //G4ThreeVector offset(ConfigManager::GetDetectorWidth()/2., ConfigManager::GetDetectorHeight()/2., ConfigManager::GetDetectorLength()/2.);

  new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
                    detector_logic_vol, "detector.physical", world_logic_vol, false, 0, true);
  //////////////////////////////////////////////////////////

  return world_phys_vol;
}

void DetectorConstruction::ConstructSDandField()
{
  // SENSITIVE DETECTOR ////////////////////////////////////

  TrackingSD* tracking_sd = new TrackingSD("/G4QPIX/TRACKING", "TrackingHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(tracking_sd);

  G4LogicalVolume* detector_logic_vol =
    G4LogicalVolumeStore::GetInstance()->GetVolume("detector.logical");

  SetSensitiveDetector(detector_logic_vol, tracking_sd);

  //////////////////////////////////////////////////////////
}
