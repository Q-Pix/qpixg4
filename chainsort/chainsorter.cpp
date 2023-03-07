// cpp
#include <string>
#include <vector>
#include <iostream>

// ROOT
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

TTree* addTree(const std::string& input_file,
    int&    m_hit_track_id_,
    double& m_hit_start_x_,
    double& m_hit_start_y_,
    double& m_hit_start_z_,
    double& m_hit_start_t_,
    double& m_hit_end_x_,
    double& m_hit_end_y_,
    double& m_hit_end_z_,
    double& m_hit_end_t_,
    double& m_hit_length_,
    double& m_hit_energy_deposit_,
    int& m_hit_process_key_,
    int& m_particle_id)
{
    TFile* tf = new TFile(input_file.data(), "READ");
    TTree* tt = (TTree*)tf->Get("event_tree");
    tt->SetBranchAddress("hit_track_id",       &m_hit_track_id_);
    tt->SetBranchAddress("hit_start_x",        &m_hit_start_x_);
    tt->SetBranchAddress("hit_start_y",        &m_hit_start_y_);
    tt->SetBranchAddress("hit_start_z",        &m_hit_start_z_);
    tt->SetBranchAddress("hit_start_t",        &m_hit_start_t_);
    tt->SetBranchAddress("hit_end_x",          &m_hit_end_x_);
    tt->SetBranchAddress("hit_end_y",          &m_hit_end_y_);
    tt->SetBranchAddress("hit_end_z",          &m_hit_end_z_);
    tt->SetBranchAddress("hit_end_t",          &m_hit_end_t_);
    tt->SetBranchAddress("hit_energy_deposit", &m_hit_energy_deposit_);
    tt->SetBranchAddress("hit_length",         &m_hit_length_);
    tt->SetBranchAddress("hit_process_key",    &m_hit_process_key_);
    tt->SetBranchAddress("particle_id",        &m_particle_id);
    return tt;
}

// iterate all through the trees sorting and filling as appropriate
void FillChainTree(std::vector<TTree*>& v_tts, TTree* output_tree, 
                int*    p_hit_track_id_, double* p_hit_start_x_,
                double* p_hit_start_y_, double* p_hit_start_z_,
                double* p_hit_start_t_, double* p_hit_end_x_,
                double* p_hit_end_y_, double* p_hit_end_z_,
                double* p_hit_end_t_, double* p_hit_length_,
                double* p_hit_energy_deposit_, int* p_hit_process_key_,
                int* p_particle_id)
{
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
    std::cout << "setting branches..\n";
    output_tree->Branch("hit_track_id",       &m_hit_track_id_);
    output_tree->Branch("hit_start_x",        &m_hit_start_x_);
    output_tree->Branch("hit_start_y",        &m_hit_start_y_);
    output_tree->Branch("hit_start_z",        &m_hit_start_z_);
    output_tree->Branch("hit_start_t",        &m_hit_start_t_);
    output_tree->Branch("hit_end_x",          &m_hit_end_x_);
    output_tree->Branch("hit_end_y",          &m_hit_end_y_);
    output_tree->Branch("hit_end_z",          &m_hit_end_z_);
    output_tree->Branch("hit_end_t",          &m_hit_end_t_);
    output_tree->Branch("hit_energy_deposit", &m_hit_energy_deposit_);
    output_tree->Branch("hit_length",         &m_hit_length_);
    output_tree->Branch("hit_process_key",    &m_hit_process_key_);
    output_tree->Branch("particle_id",        &m_particle_id);

    // build the total entries, and start each tree at zero
    long int totalEntries(0);
    std::vector<int> v_curEntry;
    std::vector<bool> b_curEntry;
    std::vector<int> v_maxEntries;
    std::cout << "beginning index building.. for: " << v_tts.size() << "\n";
    for(auto tt : v_tts){
        totalEntries += tt->GetEntries();
        std::cout << "Creating sorted tree of max entries: " << totalEntries << std::endl;
        v_maxEntries.push_back(tt->GetEntries());
        tt->GetEntry(0);
        v_curEntry.push_back(0);
        b_curEntry.push_back(true);
    }
    std::cout << "Found a total of " << v_tts.size() << "input Trees\n";

    // continue iterating through the tree vector, filling every basic hit
    long int curEntry(0);
    while(curEntry < totalEntries)
    {
        double lowest_t = MAXFLOAT;
        int k = 0;

        // figure out which tree has the lowest time
        for(int i=0; i<(int)v_tts.size(); ++i){
            if(b_curEntry[i] && p_hit_start_t_[i] < lowest_t){
                lowest_t = p_hit_start_t_[i];
                k = i;
            }
        }

        // assign and fill
        m_hit_track_id_ = p_hit_track_id_[k];
        m_hit_start_x_ = p_hit_start_x_[k];
        m_hit_start_y_ = p_hit_start_y_[k];
        m_hit_start_z_ = p_hit_start_z_[k];
        m_hit_start_t_ = p_hit_start_t_[k];
        m_hit_end_x_ = p_hit_end_x_[k];
        m_hit_end_y_ = p_hit_end_y_[k];
        m_hit_end_z_ = p_hit_end_z_[k];
        m_hit_end_t_ = p_hit_end_t_[k];
        m_hit_length_ = p_hit_length_[k];
        m_hit_energy_deposit_ = p_hit_energy_deposit_[k];
        m_hit_process_key_ = p_hit_process_key_[k];
        m_particle_id = p_particle_id[k];
        output_tree->Fill();

        // move this tree forward, and flag if we've reached the end of this tree
        v_tts[k]->GetEntry(v_curEntry[k]++);
        if(v_curEntry[k] == v_maxEntries[k]) 
            b_curEntry[k] = false;

        curEntry++;
        if(curEntry%10000 == 0)
            std::cout << "Filling sorted entry: " << curEntry << std::endl;
    }

}

// take an arbitrary list of input files with the last file being the output destination file
// create a TChain from all of the inputs, and fill the entry that has the lowest current time
int main(int argc, char** argv)
{
    std::vector<std::string> input_files;
    if(argc < 3){
        std::cout << "not enough input files!\n";
    }
    std::cout << "received number args:" << argc << "\n";
    for(int i=0; i<argc; ++i)
        std::cout << "arg " << i << ": " << argv[i] << "\n";

    // dynamic array allocation for all of the trees
    int*    m_hit_track_id_ = new int[argc-1];
    double* m_hit_start_x_ = new double[argc-1];
    double* m_hit_start_y_ = new double[argc-1];
    double* m_hit_start_z_ = new double[argc-1];
    double* m_hit_start_t_ = new double[argc-1];
    double* m_hit_end_x_ = new double[argc-1];
    double* m_hit_end_y_ = new double[argc-1];
    double* m_hit_end_z_ = new double[argc-1];
    double* m_hit_end_t_ = new double[argc-1];
    double* m_hit_length_ = new double[argc-1];
    double* m_hit_energy_deposit_ = new double[argc-1];
    int* m_hit_process_key_ = new int[argc-1];
    int* m_particle_id = new int[argc-1];

    int i=1, j=0;
    std::vector<TTree*> v_tts;
    while(i<argc-1){
        std::cout << "adding TTree at: " << argv[i] << std::endl;
        v_tts.push_back(addTree(argv[i], m_hit_track_id_[j],
                        m_hit_start_x_[j], m_hit_start_y_[j],
                        m_hit_start_z_[j], m_hit_start_t_[j],
                        m_hit_end_x_[j], m_hit_end_y_[j],
                        m_hit_end_z_[j], m_hit_end_t_[j],
                        m_hit_length_[j], m_hit_energy_deposit_[j],
                        m_hit_process_key_[j], m_particle_id[j]));
        ++i;
        ++j;
    }

    // output 
    std::cout << "creating output file at: " << argv[argc-1] << std::endl;
    std::string output_file(argv[argc-1]);
    TFile tf (output_file.data(), "RECREATE");
    TTree* tt = new TTree("event_tree", "tt");

    // fill this new tree 
    std::cout << "Filling output tree\n";
    FillChainTree(v_tts, tt, m_hit_track_id_,
                  m_hit_start_x_, m_hit_start_y_,
                  m_hit_start_z_, m_hit_start_t_,
                  m_hit_end_x_, m_hit_end_y_,
                  m_hit_end_z_, m_hit_end_t_,
                  m_hit_length_, m_hit_energy_deposit_,
                  m_hit_process_key_, m_particle_id);
    tf.Write();
    tf.Close();

    return 0;
}