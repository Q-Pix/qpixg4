#define ProtonDecayAna_cxx
#include "ProtonDecayAna.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include "TGraph.h"
using namespace std;
void ProtonDecayAna::Loop() {
//   In a ROOT session, you can do:
//      root> .L ProtonDecayAna.C
//      root> ProtonDecayAna t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();
//TH2F *h2=new TH2F("h","Kaon",100,0,100,100,0,100);
    TGraph *gr;
    Long64_t nbytes = 0, nb = 0;
    std::vector<double> range;
    std::vector<double> dedx;
    for (Long64_t jentry = 0; jentry < nentries; jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);
        nbytes += nb;
        // if (Cut(ientry) < 0) continue;
        /*cout << "NumberofParticles -> " << number_particles << endl;
        cout << "PdgCodeSize -> " << particle_pdg_code->size() << endl;
        cout << "HitDepositSize -> " << hit_energy_deposit->size() << endl;
        cout << " NumberofHits ->" << number_hits << endl;
        */
         int piontrackIds;
        float Range;
        for (int i = 0; i < number_particles; i++) {
            if (particle_pdg_code->at(i) == 321) piontrackIds = particle_track_id->at(i);

        }
        double hitE = 0;
        double hitlength=0;
        double fde=0;

        std::vector<double> E_;
        std::vector<double> X_;
        std::vector<double> Y_;
        std::vector<double> Z_;
        std::vector<double> T_;
        std::vector<double> length_;

        int CountPionHits=0;

        for (int i = 0; i < number_hits; i++) {

            if (hit_track_id->at(i) == piontrackIds) {
                //dedx.push_back(hit_energy_deposit->at(i)/hit_length->at(i));

                E_.push_back(hit_energy_deposit->at(i));
                X_.push_back(hit_start_x->at(i));
                Y_.push_back(hit_start_y->at(i));
                Z_.push_back(hit_start_z->at(i));
                length_.push_back(hit_length->at(i));
                //cout<<"E -> " <<hit_energy_deposit->at(i)<< " L -> " << hit_length->at(i)<<endl;
               // cout<<"T "<< hit_start_t->at(i)<<" X -> " <<hit_start_x->at(i)<< " Y -> " << hit_start_y->at(i)<< " Z -> " << hit_start_z->at(i)<<endl;
                CountPionHits++;

            }


            // cout<<"Event-> " <<jentry<<endl;
            //cout<<"HitDeposit-> "<<hitE<<endl;
            //h1->Fill(hitE);


        }
        float dx;
        float tempHitLength;
        if(CountPionHits>14){

            for(int i=0;i<E_.size()-1;i++){

                //cout<<"de/dx -> "<<(E_.at(i)-E_.at(i+1))/(X_.at(i)-X_.at(i+1))<<endl;
                //dx=sqrt((X_.at(i)-X_.at(i+1))*(X_.at(i)-X_.at(i+1)) + (Y_.at(i)-Y_.at(i+1))*(Y_.at(i)-Y_.at(i+1))+(Z_.at(i)-Z_.at(i+1))*(Z_.at(i)-Z_.at(i+1)) );
                //dedx.push_back(abs(E_.at(i)-E_.at(i+1))/dx);
                //dedx.push_back((E_.at(i))/dx);
                dedx.push_back(E_.at(i)/length_.at(i));
                tempHitLength=0;

                for (int k=i;k<X_.size()-1;k++){
                    //tempHitLength+=sqrt((X_.at(k)-X_.at(k+1))*(X_.at(k)-X_.at(k+1)) + (Y_.at(k)-Y_.at(k+1))*(Y_.at(k)-Y_.at(k+1))+(Z_.at(k)-Z_.at(k+1))*(Z_.at(k)-Z_.at(k+1)) );
                    tempHitLength+=length_.at(k);
                }

                range.push_back(tempHitLength);
            }

        }
        /*if (jentry==2000)
            break;
        */

    }
    gr = new TGraph(dedx.size(), &range[0], &dedx[0]);
    gr->SetMinimum(0);
    gr->SetMaximum(60);
    gr->GetXaxis()->SetLimits(0,40);

    gr->Draw("AP");
}
