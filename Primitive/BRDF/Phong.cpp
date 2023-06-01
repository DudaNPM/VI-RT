//
//  Phong.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 07/02/2023.
//


#include "Phong.hpp"


Phong::Phong(OurRGB Ka, OurRGB Kd, OurRGB Ks, OurRGB Kt, float Ns) {
    this->Ka = Ka;
    this->Kd = Kd;
    this->Ks = Ks;
    this->Kt = Kt;
    this->Ns = Ns;
}
