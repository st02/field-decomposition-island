#ifndef DATA_H
#define DATA_H

#include <array>

namespace FDI{ 
  typedef std::array<int,4> s4d;
  typedef std::array<std::array<int,3>,3> in3d; 

  typedef struct{
    s4d  coords;
    in3d intervals; // start,end,size
  }Geometry;

  enum DecompositionMode{Decompose,Refinement};
};



#endif // DATA_H
