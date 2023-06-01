//
//  RGB.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//


#ifndef RGB_hpp
#define RGB_hpp

#include <iostream>


class OurRGB {
public:
    float R, G, B;
    OurRGB ():R(0.),G(0.),B(0.) {}
    OurRGB (float r, float g, float b):R(r),G(g),B(b) {}
    OurRGB (float *rgb):R(rgb[0]),G(rgb[1]),B(rgb[2]) {}
    ~OurRGB () {}
    OurRGB& operator+=(const OurRGB& rhs){
        this->R += rhs.R;
        this->G += rhs.G;
        this->B += rhs.B;
        return *this;
    }
    OurRGB operator+(OurRGB const& obj)
    {
        OurRGB res;
        res.R = R + obj.R;
        res.G = G + obj.G;
        res.B = B + obj.B;
        return res;
    }
    OurRGB operator*(OurRGB const& obj)
    {
        OurRGB res;
        res.R = R * obj.R;
        res.G = G * obj.G;
        res.B = B * obj.B;
        return res;
    }
    OurRGB operator*(float x)
    {
        OurRGB res;
        res.R = R * x;
        res.G = G * x;
        res.B = B * x;
        return res;
    }
    
    inline OurRGB operator*(float s) const {
        return OurRGB(this->R * s, this->G * s, this->B * s);
    }
    OurRGB operator/(OurRGB const& obj)
    {
        OurRGB res;
        res.R = R / obj.R;
        res.G = G / obj.G;
        res.B = B / obj.B;
        return res;
    }
    OurRGB operator/(float x)
    {
        OurRGB res;
        res.R = R / x;
        res.G = G / x;
        res.B = B / x;
        return res;
    }
    inline OurRGB &operator*=(const OurRGB &rhs) {
        this->R *= rhs.R;
        this->G *= rhs.G;
        this->B *= rhs.B;
        return *this;
    }
    inline OurRGB &operator*=(float s) {
        this->R *= s;
        this->G *= s;
        this->B *= s;
        return *this;
    }

    float Y() {
        return (R*(float)0.2126 + G*(float)0.7152 + B*(float)0.0722);
    }
    bool isZero () {
        return ((R==0.) && (G==0.) && (B==0.));
    }
};

inline OurRGB operator*(float s, const OurRGB &c) { return c * s; }

#endif /* RGB_hpp */
