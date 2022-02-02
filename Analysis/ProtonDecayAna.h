//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Feb  1 12:22:59 2022 by ROOT version 6.24/06
// from TTree event_tree/event tree
// found on file: Proton_Decay.root
//////////////////////////////////////////////////////////

#ifndef ProtonDecayAna_h
#define ProtonDecayAna_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"

class ProtonDecayAna {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           run;
   Int_t           event;
   Int_t           generator_initial_number_particles;
   vector<double>  *generator_initial_particle_x;
   vector<double>  *generator_initial_particle_y;
   vector<double>  *generator_initial_particle_z;
   vector<double>  *generator_initial_particle_t;
   vector<double>  *generator_initial_particle_px;
   vector<double>  *generator_initial_particle_py;
   vector<double>  *generator_initial_particle_pz;
   vector<double>  *generator_initial_particle_energy;
   vector<int>     *generator_initial_particle_pdg_code;
   vector<double>  *generator_initial_particle_mass;
   vector<double>  *generator_initial_particle_charge;
   Int_t           generator_final_number_particles;
   vector<double>  *generator_final_particle_x;
   vector<double>  *generator_final_particle_y;
   vector<double>  *generator_final_particle_z;
   vector<double>  *generator_final_particle_t;
   vector<double>  *generator_final_particle_px;
   vector<double>  *generator_final_particle_py;
   vector<double>  *generator_final_particle_pz;
   vector<double>  *generator_final_particle_energy;
   vector<int>     *generator_final_particle_pdg_code;
   vector<double>  *generator_final_particle_mass;
   vector<double>  *generator_final_particle_charge;
   Int_t           number_particles;
   Int_t           number_hits;
   Double_t        energy_deposit;
   vector<int>     *particle_track_id;
   vector<int>     *particle_parent_track_id;
   vector<int>     *particle_pdg_code;
   vector<double>  *particle_mass;
   vector<double>  *particle_charge;
   vector<int>     *particle_process_key;
   vector<int>     *particle_total_occupancy;
   vector<double>  *particle_initial_x;
   vector<double>  *particle_initial_y;
   vector<double>  *particle_initial_z;
   vector<double>  *particle_initial_t;
   vector<double>  *particle_initial_px;
   vector<double>  *particle_initial_py;
   vector<double>  *particle_initial_pz;
   vector<double>  *particle_initial_energy;
   vector<int>     *particle_number_daughters;
   vector<vector<int> > *particle_daughter_track_id;
   vector<int>     *hit_track_id;
   vector<double>  *hit_start_x;
   vector<double>  *hit_start_y;
   vector<double>  *hit_start_z;
   vector<double>  *hit_start_t;
   vector<double>  *hit_end_x;
   vector<double>  *hit_end_y;
   vector<double>  *hit_end_z;
   vector<double>  *hit_end_t;
   vector<double>  *hit_energy_deposit;
   vector<double>  *hit_length;
   vector<int>     *hit_process_key;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_generator_initial_number_particles;   //!
   TBranch        *b_generator_initial_particle_x;   //!
   TBranch        *b_generator_initial_particle_y;   //!
   TBranch        *b_generator_initial_particle_z;   //!
   TBranch        *b_generator_initial_particle_t;   //!
   TBranch        *b_generator_initial_particle_px;   //!
   TBranch        *b_generator_initial_particle_py;   //!
   TBranch        *b_generator_initial_particle_pz;   //!
   TBranch        *b_generator_initial_particle_energy;   //!
   TBranch        *b_generator_initial_particle_pdg_code;   //!
   TBranch        *b_generator_initial_particle_mass;   //!
   TBranch        *b_generator_initial_particle_charge;   //!
   TBranch        *b_generator_final_number_particles;   //!
   TBranch        *b_generator_final_particle_x;   //!
   TBranch        *b_generator_final_particle_y;   //!
   TBranch        *b_generator_final_particle_z;   //!
   TBranch        *b_generator_final_particle_t;   //!
   TBranch        *b_generator_final_particle_px;   //!
   TBranch        *b_generator_final_particle_py;   //!
   TBranch        *b_generator_final_particle_pz;   //!
   TBranch        *b_generator_final_particle_energy;   //!
   TBranch        *b_generator_final_particle_pdg_code;   //!
   TBranch        *b_generator_final_particle_mass;   //!
   TBranch        *b_generator_final_particle_charge;   //!
   TBranch        *b_number_particles;   //!
   TBranch        *b_number_hits;   //!
   TBranch        *b_energy_deposit;   //!
   TBranch        *b_particle_track_id;   //!
   TBranch        *b_particle_parent_track_id;   //!
   TBranch        *b_particle_pdg_code;   //!
   TBranch        *b_particle_mass;   //!
   TBranch        *b_particle_charge;   //!
   TBranch        *b_particle_process_key;   //!
   TBranch        *b_particle_total_occupancy;   //!
   TBranch        *b_particle_initial_x;   //!
   TBranch        *b_particle_initial_y;   //!
   TBranch        *b_particle_initial_z;   //!
   TBranch        *b_particle_initial_t;   //!
   TBranch        *b_particle_initial_px;   //!
   TBranch        *b_particle_initial_py;   //!
   TBranch        *b_particle_initial_pz;   //!
   TBranch        *b_particle_initial_energy;   //!
   TBranch        *b_particle_number_daughters;   //!
   TBranch        *b_particle_daughter_track_id;   //!
   TBranch        *b_hit_track_id;   //!
   TBranch        *b_hit_start_x;   //!
   TBranch        *b_hit_start_y;   //!
   TBranch        *b_hit_start_z;   //!
   TBranch        *b_hit_start_t;   //!
   TBranch        *b_hit_end_x;   //!
   TBranch        *b_hit_end_y;   //!
   TBranch        *b_hit_end_z;   //!
   TBranch        *b_hit_end_t;   //!
   TBranch        *b_hit_energy_deposit;   //!
   TBranch        *b_hit_length;   //!
   TBranch        *b_hit_process_key;   //!

   ProtonDecayAna(TTree *tree=0);
   virtual ~ProtonDecayAna();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ProtonDecayAna_cxx
ProtonDecayAna::ProtonDecayAna(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/media/argon/DATA/Ilker/Proton_Decay.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/media/argon/DATA/Ilker/Proton_Decay.root");
      }
      f->GetObject("event_tree",tree);

   }
   Init(tree);
}

ProtonDecayAna::~ProtonDecayAna()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ProtonDecayAna::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ProtonDecayAna::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ProtonDecayAna::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   generator_initial_particle_x = 0;
   generator_initial_particle_y = 0;
   generator_initial_particle_z = 0;
   generator_initial_particle_t = 0;
   generator_initial_particle_px = 0;
   generator_initial_particle_py = 0;
   generator_initial_particle_pz = 0;
   generator_initial_particle_energy = 0;
   generator_initial_particle_pdg_code = 0;
   generator_initial_particle_mass = 0;
   generator_initial_particle_charge = 0;
   generator_final_particle_x = 0;
   generator_final_particle_y = 0;
   generator_final_particle_z = 0;
   generator_final_particle_t = 0;
   generator_final_particle_px = 0;
   generator_final_particle_py = 0;
   generator_final_particle_pz = 0;
   generator_final_particle_energy = 0;
   generator_final_particle_pdg_code = 0;
   generator_final_particle_mass = 0;
   generator_final_particle_charge = 0;
   particle_track_id = 0;
   particle_parent_track_id = 0;
   particle_pdg_code = 0;
   particle_mass = 0;
   particle_charge = 0;
   particle_process_key = 0;
   particle_total_occupancy = 0;
   particle_initial_x = 0;
   particle_initial_y = 0;
   particle_initial_z = 0;
   particle_initial_t = 0;
   particle_initial_px = 0;
   particle_initial_py = 0;
   particle_initial_pz = 0;
   particle_initial_energy = 0;
   particle_number_daughters = 0;
   particle_daughter_track_id = 0;
   hit_track_id = 0;
   hit_start_x = 0;
   hit_start_y = 0;
   hit_start_z = 0;
   hit_start_t = 0;
   hit_end_x = 0;
   hit_end_y = 0;
   hit_end_z = 0;
   hit_end_t = 0;
   hit_energy_deposit = 0;
   hit_length = 0;
   hit_process_key = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("generator_initial_number_particles", &generator_initial_number_particles, &b_generator_initial_number_particles);
   fChain->SetBranchAddress("generator_initial_particle_x", &generator_initial_particle_x, &b_generator_initial_particle_x);
   fChain->SetBranchAddress("generator_initial_particle_y", &generator_initial_particle_y, &b_generator_initial_particle_y);
   fChain->SetBranchAddress("generator_initial_particle_z", &generator_initial_particle_z, &b_generator_initial_particle_z);
   fChain->SetBranchAddress("generator_initial_particle_t", &generator_initial_particle_t, &b_generator_initial_particle_t);
   fChain->SetBranchAddress("generator_initial_particle_px", &generator_initial_particle_px, &b_generator_initial_particle_px);
   fChain->SetBranchAddress("generator_initial_particle_py", &generator_initial_particle_py, &b_generator_initial_particle_py);
   fChain->SetBranchAddress("generator_initial_particle_pz", &generator_initial_particle_pz, &b_generator_initial_particle_pz);
   fChain->SetBranchAddress("generator_initial_particle_energy", &generator_initial_particle_energy, &b_generator_initial_particle_energy);
   fChain->SetBranchAddress("generator_initial_particle_pdg_code", &generator_initial_particle_pdg_code, &b_generator_initial_particle_pdg_code);
   fChain->SetBranchAddress("generator_initial_particle_mass", &generator_initial_particle_mass, &b_generator_initial_particle_mass);
   fChain->SetBranchAddress("generator_initial_particle_charge", &generator_initial_particle_charge, &b_generator_initial_particle_charge);
   fChain->SetBranchAddress("generator_final_number_particles", &generator_final_number_particles, &b_generator_final_number_particles);
   fChain->SetBranchAddress("generator_final_particle_x", &generator_final_particle_x, &b_generator_final_particle_x);
   fChain->SetBranchAddress("generator_final_particle_y", &generator_final_particle_y, &b_generator_final_particle_y);
   fChain->SetBranchAddress("generator_final_particle_z", &generator_final_particle_z, &b_generator_final_particle_z);
   fChain->SetBranchAddress("generator_final_particle_t", &generator_final_particle_t, &b_generator_final_particle_t);
   fChain->SetBranchAddress("generator_final_particle_px", &generator_final_particle_px, &b_generator_final_particle_px);
   fChain->SetBranchAddress("generator_final_particle_py", &generator_final_particle_py, &b_generator_final_particle_py);
   fChain->SetBranchAddress("generator_final_particle_pz", &generator_final_particle_pz, &b_generator_final_particle_pz);
   fChain->SetBranchAddress("generator_final_particle_energy", &generator_final_particle_energy, &b_generator_final_particle_energy);
   fChain->SetBranchAddress("generator_final_particle_pdg_code", &generator_final_particle_pdg_code, &b_generator_final_particle_pdg_code);
   fChain->SetBranchAddress("generator_final_particle_mass", &generator_final_particle_mass, &b_generator_final_particle_mass);
   fChain->SetBranchAddress("generator_final_particle_charge", &generator_final_particle_charge, &b_generator_final_particle_charge);
   fChain->SetBranchAddress("number_particles", &number_particles, &b_number_particles);
   fChain->SetBranchAddress("number_hits", &number_hits, &b_number_hits);
   fChain->SetBranchAddress("energy_deposit", &energy_deposit, &b_energy_deposit);
   fChain->SetBranchAddress("particle_track_id", &particle_track_id, &b_particle_track_id);
   fChain->SetBranchAddress("particle_parent_track_id", &particle_parent_track_id, &b_particle_parent_track_id);
   fChain->SetBranchAddress("particle_pdg_code", &particle_pdg_code, &b_particle_pdg_code);
   fChain->SetBranchAddress("particle_mass", &particle_mass, &b_particle_mass);
   fChain->SetBranchAddress("particle_charge", &particle_charge, &b_particle_charge);
   fChain->SetBranchAddress("particle_process_key", &particle_process_key, &b_particle_process_key);
   fChain->SetBranchAddress("particle_total_occupancy", &particle_total_occupancy, &b_particle_total_occupancy);
   fChain->SetBranchAddress("particle_initial_x", &particle_initial_x, &b_particle_initial_x);
   fChain->SetBranchAddress("particle_initial_y", &particle_initial_y, &b_particle_initial_y);
   fChain->SetBranchAddress("particle_initial_z", &particle_initial_z, &b_particle_initial_z);
   fChain->SetBranchAddress("particle_initial_t", &particle_initial_t, &b_particle_initial_t);
   fChain->SetBranchAddress("particle_initial_px", &particle_initial_px, &b_particle_initial_px);
   fChain->SetBranchAddress("particle_initial_py", &particle_initial_py, &b_particle_initial_py);
   fChain->SetBranchAddress("particle_initial_pz", &particle_initial_pz, &b_particle_initial_pz);
   fChain->SetBranchAddress("particle_initial_energy", &particle_initial_energy, &b_particle_initial_energy);
   fChain->SetBranchAddress("particle_number_daughters", &particle_number_daughters, &b_particle_number_daughters);
   fChain->SetBranchAddress("particle_daughter_track_id", &particle_daughter_track_id, &b_particle_daughter_track_id);
   fChain->SetBranchAddress("hit_track_id", &hit_track_id, &b_hit_track_id);
   fChain->SetBranchAddress("hit_start_x", &hit_start_x, &b_hit_start_x);
   fChain->SetBranchAddress("hit_start_y", &hit_start_y, &b_hit_start_y);
   fChain->SetBranchAddress("hit_start_z", &hit_start_z, &b_hit_start_z);
   fChain->SetBranchAddress("hit_start_t", &hit_start_t, &b_hit_start_t);
   fChain->SetBranchAddress("hit_end_x", &hit_end_x, &b_hit_end_x);
   fChain->SetBranchAddress("hit_end_y", &hit_end_y, &b_hit_end_y);
   fChain->SetBranchAddress("hit_end_z", &hit_end_z, &b_hit_end_z);
   fChain->SetBranchAddress("hit_end_t", &hit_end_t, &b_hit_end_t);
   fChain->SetBranchAddress("hit_energy_deposit", &hit_energy_deposit, &b_hit_energy_deposit);
   fChain->SetBranchAddress("hit_length", &hit_length, &b_hit_length);
   fChain->SetBranchAddress("hit_process_key", &hit_process_key, &b_hit_process_key);
   Notify();
}

Bool_t ProtonDecayAna::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ProtonDecayAna::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ProtonDecayAna::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ProtonDecayAna_cxx
