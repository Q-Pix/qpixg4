// ROOT includes
#include "Math/SVector.h"
#include "Math/SMatrix.h" 

ROOT::Math::SMatrix< double, 3 > Rotation_Matrix(std::vector<double> vec1, std::vector<double> vec2)
{
  // --------------------------------------------------------------------------
  // Rotation matrix r that rotates vector vec1 onto vector vec2
  // https://math.stackexchange.com/a/476311
  // --------------------------------------------------------------------------
  
  double identity_array[9] = { 1, 0, 0,
                              0, 1, 0,
                              0, 0, 1  };

  ROOT::Math::SMatrix< double, 3 > I(identity_array, 9);
  ROOT::Math::SMatrix< double, 3 > r = I;

  double b_array[3] = { vec2[0],
                        vec2[1],
                        vec2[2]  };
  ROOT::Math::SVector< double, 3 > b(b_array, 3);
  b = b.Unit();

  double a_array[3] = { vec1[0],
                        vec1[1],
                        vec1[2] };

  ROOT::Math::SVector< double, 3 > a(a_array, 3);
  a = a.Unit();

  double cosine = ROOT::Math::Dot(a, b);

  // cross product between a and b
  ROOT::Math::SVector< double, 3 > v = ROOT::Math::Cross(a, b);

  // sine between a and b
  double sine = ROOT::Math::Mag(v);

  // skew-symmetric cross-product matrix of v
  double V_array[9] = {     0, -v(2),  v(1),
                        v(2),     0, -v(0),
                        -v(1),  v(0),     0  };
  ROOT::Math::SMatrix< double, 3 > V(V_array, 9);
  
  // rotation matrix
  r = I + V + V*V*(1-cosine)/sine/sine;


  return r;
}

// run the test rotate code for some example vectors
void testRotate()
{
    std::vector<std::vector<double>> p1 = {{1,0,0}, {0,1,0}, {0,0,1}};

    ROOT::Math::SVector<double, 3> net_p;
    for(auto p : p1){
        net_p[0] += p[0];
        net_p[1] += p[1];
        net_p[2] += p[2];
    }
    net_p = net_p.Unit();

    ROOT::Math::SVector<double, 3> rs;
    rs[0] = 0;
    rs[1] = 1;
    rs[2] = 0;

    std::vector<double> v1({rs[0], rs[1], rs[2]}), v2({net_p[0], net_p[1], net_p[2]});
    ROOT::Math::SMatrix< double, 3 > rot_matrix = Rotation_Matrix(v2, v1); // rotate momentum onto rs

    // rotate to the new vector
    for(auto p : p1){
        std::cout << "x: " << p[0] << ", y: " << p[1] << ", z:" << p[2] << std::endl;
        ROOT::Math::SVector<double, 3> init_p({p[0], p[1], p[2]});
        // rotation happens here
        ROOT::Math::SVector<double, 3> final_p = rot_matrix * init_p;

        // set new values
        std::cout << "x: " << final_p[0] << ", y: " << final_p[1] << ", z:" << final_p[2] << std::endl;
    }

};