// -----------------------------------------------------------------------------
//  MCParticle.h
//
//  Class definition of MCParticle
//   * Author: Everybody is an author!
//   * Creation date: 7 August 2020
// -----------------------------------------------------------------------------

#ifndef MCParticle_h
#define MCParticle_h 1

// Q-Pix includes
#include "geo_types.h"

// GEANT4 includes
#include "G4LorentzVector.hh"
#include "G4Step.hh"

// ROOT includes
#include "TLorentzVector.h"

// C++ includes
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// struct TrajectoryPoint
// {
//     TLorentzVector position_;
//     TLorentzVector momentum_;
//     std::string    process_ = "unknown";
//     double         energy_deposit_ = 0;
//     double         length_ = 0;
// 
//     TLorentzVector Position()      const { return position_;       }
//     TLorentzVector Momentum()      const { return momentum_;       }
//     std::string    Process()       const { return process_;        }
//     double         EnergyDeposit() const { return energy_deposit_; }
//     double         Length()        const { return length_;         }
// 
//     double X()  const { return position_.X();  }
//     double Y()  const { return position_.Y();  }
//     double Z()  const { return position_.Z();  }
//     double T()  const { return position_.T();  }
// 
//     double Px() const { return momentum_.Px(); }
//     double Py() const { return momentum_.Py(); }
//     double Pz() const { return momentum_.Pz(); }
//     double E()  const { return momentum_.E();  }
// };

struct TrajectoryHit
{
    Point_t     start_ = {0., 0., 0.};
    Point_t     end_ = {0., 0., 0.};
    double      energy_deposit_ = 0;
    double      start_time_ = 0;
    double      end_time_ = 0;
    int         track_id_ = -1;
    int         pdg_code_ = -1;
    double      length_ = 0;
    std::string process_ = "unknown";

    Point_t StartPoint()  const { return start_; }
    Point_t EndPoint()    const { return end_;   }
    Point_t MidPoint()    const { return { (start_.X() + end_.X())/2.0,
                                           (start_.Y() + end_.Y())/2.0,
                                           (start_.Z() + end_.Z())/2.0 }; }
    double  StartTime()   const { return start_time_;     }
    double  EndTime()     const { return end_time_;       }
    double  Time()        const { return (start_time_ + end_time_)/2.0; }
    double  Energy()      const { return energy_deposit_; }
    double  TrackID()     const { return track_id_;       }
    double  PDGCode()     const { return pdg_code_;       }
    double  Length()      const { return length_;         }
    std::string Process() const { return process_; }
};

class MCParticle
{

    public:

        MCParticle();
        ~MCParticle();

        // void AddTrajectoryPoint(const TrajectoryPoint &);
        void AddTrajectoryHit(TrajectoryHit const &);
        void AddTrajectoryHit(G4Step const *);

        void AddDaughter(int const);

        inline std::vector< TrajectoryHit > Hits() const { return hits_; }

        inline int          NumberDaughters() const { return number_daughters_; }
        inline std::vector< int > Daughters() const { return daughter_track_ids_; }

        inline int         TrackID()        const { return track_id_;        }
        inline int         ParentTrackID()  const { return parent_track_id_; }
        inline int         PDGCode()        const { return pdg_code_;        }
        inline double      Mass()           const { return mass_;            }
        inline double      Charge()         const { return charge_;          }
        inline double      GlobalTime()     const { return global_time_;     }
        inline std::string Process()        const { return process_;         }
        inline int         TotalOccupancy() const { return total_occupancy_; }

        inline double EnergyDeposited() const { return energy_deposited_; }

        inline TLorentzVector InitialPosition() const { return initial_position_; }
        inline TLorentzVector InitialMomentum() const { return initial_momentum_; }

        inline void SetTrackID(int const trackID)               { track_id_ = trackID;               }
        inline void SetParentTrackID(int const parentTrackID)   { parent_track_id_ = parentTrackID;  }
        inline void SetPDGCode(int const pdgCode)               { pdg_code_ = pdgCode;               }
        inline void SetMass(double const mass)                  { mass_ = mass;                      }
        inline void SetCharge(double const charge)              { charge_ = charge;                  }
        inline void SetGlobalTime(double const globalTime)      { global_time_ = globalTime;         }
        inline void SetProcess(std::string const process)       { process_ = process;                }
        inline void SetTotalOccupancy(int const totalOccupancy) { total_occupancy_ = totalOccupancy; }

        inline void SetInitialPosition(TLorentzVector const initialPosition) { initial_position_ = initialPosition; }
        inline void SetInitialMomentum(TLorentzVector const initialMomentum) { initial_momentum_ = initialMomentum; }

    private:

        int         track_id_;
        int         parent_track_id_;
        int         pdg_code_;
        double      mass_;
        double      charge_;
        double      global_time_;
        std::string process_;
        int         total_occupancy_;

        double      energy_deposited_ = 0.;

        TLorentzVector initial_position_;
        TLorentzVector initial_momentum_;

        // std::vector< TrajectoryPoint > trajectory_;
        std::vector< TrajectoryHit > hits_;

        int                number_daughters_ = 0;
        std::vector< int > daughter_track_ids_;

};

#endif

