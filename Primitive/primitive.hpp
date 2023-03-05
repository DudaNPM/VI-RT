//
//  primitive.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef primitive_hpp
#define primitive_hpp

#include "geometry.hpp"
#include "BRDF.hpp"

typedef struct Primitive {
    std::shared_ptr<Geometry> g;
    int material_ndx;
} Primitive;

#endif /* primitive_hpp */
