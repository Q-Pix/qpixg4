// -----------------------------------------------------------------------------
//  G4_QPIX | DetectorConstruction.h
//
//  Class for the definition of the detector geometry and materials.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#ifndef DETECTOR_CONSTRUCTION_H
#define DETECTOR_CONSTRUCTION_H 1

#include "G4VUserDetectorConstruction.hh"
#include "G4GenericMessenger.hh"
class G4Material;


class DetectorConstruction: public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  private:
    G4LogicalVolume* detector_logic_vol;
    G4LogicalVolume* BuildBoxDetector();
    G4LogicalVolume* BuildHDDetector();
    G4LogicalVolume* BuildVDDetector();
    G4LogicalVolume* BuildTSDetector();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
};

#endif
