// -----------------------------------------------------------------------------
//  GeneratorParticle.h
//
//  Class definition of GeneratorParticle
//   * Author: Everybody is an author!
//   * Creation date: 4 September 2020
// -----------------------------------------------------------------------------

#ifndef GeneratorParticle_h
#define GeneratorParticle_h 1

class GeneratorParticle {

    public:

        GeneratorParticle();
        ~GeneratorParticle();

        inline int     PDGCode() const { return pdg_code_; }
        inline double  Mass()    const { return mass_;     }
        inline double  Charge()  const { return charge_;   }
        inline double  X()       const { return x_;        }
        inline double  Y()       const { return y_;        }
        inline double  Z()       const { return z_;        }
        inline double  T()       const { return t_;        }
        inline double  Px()      const { return px_;       }
        inline double  Py()      const { return py_;       }
        inline double  Pz()      const { return pz_;       }
        inline double  Energy()  const { return energy_;   }

        inline void SetPDGCode (int    const pdgCode) { pdg_code_ = pdgCode; }
        inline void SetMass    (double const mass   ) { mass_     = mass;    }
        inline void SetCharge  (double const charge ) { charge_   = charge;  }
        inline void SetX       (double const x      ) { x_        = x;       }
        inline void SetY       (double const y      ) { y_        = y;       }
        inline void SetZ       (double const z      ) { z_        = z;       }
        inline void SetT       (double const t      ) { t_        = t;       }
        inline void SetPx      (double const px     ) { px_       = px;      }
        inline void SetPy      (double const py     ) { py_       = py;      }
        inline void SetPz      (double const pz     ) { pz_       = pz;      }
        inline void SetEnergy  (double const energy ) { energy_   = energy;  }

    private:

        int    pdg_code_;
        double mass_;
        double charge_;

        double x_;
        double y_;
        double z_;
        double t_;

        double px_;
        double py_;
        double pz_;
        double energy_;

};

#endif
