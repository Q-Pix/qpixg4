#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include <vector>

// ## Preparation
#include <numeric>      // std::iota
#include <algorithm>    // std::sort, std::stable_sort
#include <memory>

template <typename T>
std::vector<size_t> sort_indexes(const std::vector<T> &v) {

  // initialize original index locations
  std::vector<size_t> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  // using std::stable_sort instead of std::sort
  // to avoid unnecessary index re-orderings
  // when v contains elements of equal values
  std::stable_sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});

  return idx;
}
int main(int argc, char** argv)
{
    if(argc != 3){
        std::cout << "incorrect args";
        return -1;
    }
    TFile in_tf(argv[1], "UPDATE");
    TTree* in_tt = (TTree*)in_tf.Get("event_tree");
    int    m_hit_track_id_;
    double m_hit_start_x_;
    double m_hit_start_y_;
    double m_hit_start_z_;
    double m_hit_start_t_;
    double m_hit_end_x_;
    double m_hit_end_y_;
    double m_hit_end_z_;
    double m_hit_end_t_;
    double m_hit_length_;
    double m_hit_energy_deposit_;
    int m_hit_process_key_;
    int m_particle_id;
    in_tt->SetBranchAddress("hit_track_id",       &m_hit_track_id_);
    in_tt->SetBranchAddress("hit_start_x",        &m_hit_start_x_);
    in_tt->SetBranchAddress("hit_start_y",        &m_hit_start_y_);
    in_tt->SetBranchAddress("hit_start_z",        &m_hit_start_z_);
    in_tt->SetBranchAddress("hit_start_t",        &m_hit_start_t_);
    in_tt->SetBranchAddress("hit_end_x",          &m_hit_end_x_);
    in_tt->SetBranchAddress("hit_end_y",          &m_hit_end_y_);
    in_tt->SetBranchAddress("hit_end_z",          &m_hit_end_z_);
    in_tt->SetBranchAddress("hit_end_t",          &m_hit_end_t_);
    in_tt->SetBranchAddress("hit_energy_deposit", &m_hit_energy_deposit_);
    in_tt->SetBranchAddress("hit_length",         &m_hit_length_);
    in_tt->SetBranchAddress("hit_process_key",    &m_hit_process_key_);
    in_tt->SetBranchAddress("particle_id",        &m_particle_id);

    std::cout << "building entries..\n";
    std::vector<size_t> indices;
    for(int i=0; i<in_tt->GetEntries(); ++i)
        indices.push_back(m_hit_end_t_);

    std::cout << "sorting entries..\n";
    auto sorted = sort_indexes(indices);

    // TFile out_tf(argv[2], "RECREATE");
    TTree tt("sort_event_tree", "sort_event_tree");
    tt.Branch("hit_track_id",       &m_hit_track_id_);
    tt.Branch("hit_start_x",        &m_hit_start_x_);
    tt.Branch("hit_start_y",        &m_hit_start_y_);
    tt.Branch("hit_start_z",        &m_hit_start_z_);
    tt.Branch("hit_start_t",        &m_hit_start_t_);
    tt.Branch("hit_end_x",          &m_hit_end_x_);
    tt.Branch("hit_end_y",          &m_hit_end_y_);
    tt.Branch("hit_end_z",          &m_hit_end_z_);
    tt.Branch("hit_end_t",          &m_hit_end_t_);
    tt.Branch("hit_energy_deposit", &m_hit_energy_deposit_);
    tt.Branch("hit_length",         &m_hit_length_);
    tt.Branch("hit_process_key",    &m_hit_process_key_);
    tt.Branch("particle_id",        &m_particle_id);

    int j = 0;
    std::cout << "entries sorted.. building tree..\n";
    for(auto i : sorted){
        in_tt->GetEntry(i);
        tt.Fill();
        ++j;
        if(j%100000==0) std::cout << "Filling Entry " << j << std::endl;
    }
    tt.Write();
    // out_tf.Close();

    return 0;
}