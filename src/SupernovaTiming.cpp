// -----------------------------------------------------------------------------
//  SupernovaTiming.cpp
//
//
//   * Author: Everybody is an author!
//   * Creation date: 25 September 2020
// -----------------------------------------------------------------------------

#include "SupernovaTiming.h"

//-----------------------------------------------------------------------------
SupernovaTiming::SupernovaTiming()
  : initialized_(false),
    on_(false),
    input_file_(""),
    th2_name_("nusperbin2d_nue"),
    tfile_(0)
{
    msg_ = new G4GenericMessenger(
        this, "/supernova/timing/", "control commands for SupernovaTiming");
    msg_->DeclareProperty("on", on_, "turn on SupernovaTiming");
    msg_->DeclareProperty("input_file", input_file_, "input ROOT file");
    msg_->DeclareProperty("th2_name", th2_name_, "name of TH2");
}

//-----------------------------------------------------------------------------
SupernovaTiming::~SupernovaTiming()
{
    if (initialized_ && on_)
    {
        delete th1_;
        delete th2_;
        delete tfile_;
    }
}

//-----------------------------------------------------------------------------
void SupernovaTiming::Initialize()
{
    if (on_)
    {
        G4cout << "Initializing SupernovaTiming..."
               << "\ninput_file: " << input_file_
               << "\nth2_name:   " << th2_name_
               << G4endl;

        // open supernova ROOT file for reading
        tfile_ = new TFile(input_file_.data(), "read");

        // check to see if time vs. energy histogram exists
        bool th2_status = tfile_->GetListOfKeys()->Contains(th2_name_.data());

        // throw exception if time vs. energy histogram does not exist
        if (!th2_status)
        {
            std::cerr << "\nERROR: TObject `"
                      << th2_name_
                      << "` not found in file `"
                      << input_file_
                      << "`!\n"
                      << std::endl;
            throw std::exception();
        }

        // get time vs. energy histogram
        // th2_ = (TH2D*) tfile_->Get(th2_name.data());
        tfile_->GetObject(th2_name_.data(), th2_);

        // project histogram onto energy axis
        th1_ = (TH1D*) th2_->ProjectionY();  // MeV
    }
    else
    {
        G4cout << "SupernovaTiming is turned off" << G4endl;
    }

    // set initialized flag
    initialized_ = true;
}

//-----------------------------------------------------------------------------
bool SupernovaTiming::Status()
{
    // if (!initialized_ || !on_) return false;
    if (!tfile_ || tfile_->IsZombie())
    {
        // G4cout << "SupernovaTiming::Status(): tfile_ is empty" << G4endl;
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
double SupernovaTiming::Sample(double const energy)
{
    if (!initialized_ || !on_ || !th1_ || !th2_) return 0.;

    // get energy bin
    int const bin_idx = th1_->FindBin(energy);  // energy in MeV

    // get time distribution of energy bin
    TH1D * th1_time = th2_->ProjectionX("time", bin_idx, bin_idx+1);  // sec

    // sample from time distribution
    double const time = th1_time->GetRandom();  // sec

    return time;
}

