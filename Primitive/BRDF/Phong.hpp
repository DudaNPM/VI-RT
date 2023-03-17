//
//  Phong.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 07/02/2023.
//

#ifndef Phong_hpp
#define Phong_hpp

#include "RGB.hpp"
#include "BRDF.hpp"

class Phong: public BRDF {
public:
    RGB Ka, Kd, Ks, Kt; // Tf == Kt
    float Ns;


    Phong(RGB Ka, RGB Kd, RGB Ks, RGB Kt, float Ns);
};

#endif /* Phong_hpp */
