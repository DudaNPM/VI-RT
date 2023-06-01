//
//  Phong.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 07/02/2023.
//


#ifndef Phong_hpp
#define Phong_hpp

#include "BRDF.hpp"


class Phong: public BRDF {
public:
    OurRGB Ka, Kd, Ks, Kt; // Tf == Kt
    float Ns;


    Phong(OurRGB Ka, OurRGB Kd, OurRGB Ks, OurRGB Kt, float Ns);
};

#endif /* Phong_hpp */
