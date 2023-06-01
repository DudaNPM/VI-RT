//
//  light.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 04/02/2023.
//


#ifndef light_hpp
#define light_hpp

#include "triangle.hpp"
#include "vector.hpp"
#include "RGB.hpp"


enum LightType {
    NO_LIGHT,
    AMBIENT_LIGHT,
    POINT_LIGHT,
    AREA_LIGHT,
    INFINITE_LIGHT
} ;

class Light {
public:
    LightType type;
    Light () {type=NO_LIGHT;}
    ~Light () {}
    // return the Light RGB radiance for a given point : p
    virtual OurRGB L (Point p) {return OurRGB();}
    // return the Light RGB radiance
    virtual OurRGB L () {return OurRGB();}
    // return a point p and RGB radiance for a given probability pair prob[2]
    virtual OurRGB Sample_L (float *prob, Point *p) {return OurRGB();}
    // return the probability of p
    virtual float pdf(Point p) {return 1.;}
};

#endif /* light_hpp */
