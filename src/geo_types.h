// -----------------------------------------------------------------------------
//  geo_types.h
//
//  Definitions of geo types
//   * Author: Everybody is an author!
//   * Creation date: 10 August 2020
// -----------------------------------------------------------------------------

// ROOT includes
#include "Math/GenVector/CoordinateSystemTags.h"
#include "Math/GenVector/Cartesian3D.h"
#include "Math/GenVector/PositionVector3D.h"
#include "Math/GenVector/DisplacementVector3D.h"
#include "Math/GenVector/Rotation3D.h"

using Point_t = ROOT::Math::PositionVector3D
      < ROOT::Math::Cartesian3D< double >,
        ROOT::Math::GlobalCoordinateSystemTag >;

using Vector_t = ROOT::Math::DisplacementVector3D
       < ROOT::Math::Cartesian3D<double>,
         ROOT::Math::GlobalCoordinateSystemTag >;

