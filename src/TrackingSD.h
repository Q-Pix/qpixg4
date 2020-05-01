// -----------------------------------------------------------------------------
//  G4Basic | TrackingSD.h
//
//  TODO: Class description
//   * Author: Justo Martin-Albo
//   * Creation date: 12 Feb 2019
// -----------------------------------------------------------------------------

#ifndef TRACKING_SD_H
#define TRACKING_SD_H

#include <G4VSensitiveDetector.hh>
#include "TrackingHit.h"
#include <G4GenericMessenger.hh>

#include <vector>

class TrackingSD: public G4VSensitiveDetector
{
public:
  TrackingSD(const G4String&, const G4String&);
  virtual ~TrackingSD();

  virtual void   Initialize(G4HCofThisEvent*);
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  virtual void   EndOfEvent(G4HCofThisEvent*);
  void OpenFile(G4String);

private:
  TrackingHitsCollection* hc_;
  int Acounter =0 ; 
  //std::vector<double> Event_Vector;
  std::vector< std::vector<double> > Event_Vector;
  std::vector<double> Tmp_Vector;
  std::ofstream Data_File;
  G4GenericMessenger* msg_;
  G4String file_out_;
};
#endif
