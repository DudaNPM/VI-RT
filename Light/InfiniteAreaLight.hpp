#ifndef InfiniteAreaLight_hpp
#define InfiniteAreaLight_hpp


#include "light.hpp"
#include "vector.hpp"
#include "EnvMap.hpp"
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <iomanip>

#include <math.h>

class InfiniteAreaLight: public Light {
public:
    const HDRImageBuffer* envMap;
    Point worldCenter;
    float worldRadius;

    float pdf;

    InfiniteAreaLight(HDRImageBuffer* envMap, Point worldCenter, float worldRadius){
        type = INFINITE_LIGHT;
        pdf = 1.0f / (float) (4.0f * M_PI * powf(worldRadius,2.0f)); // for uniform sampling the 
        this->worldRadius = worldRadius;
        this->worldCenter = worldCenter;
        this->envMap = envMap;
    }
    
    ~InfiniteAreaLight() {delete envMap;}
    
    inline int clamp(int value, int minVal, int maxVal) {
        return (value < minVal) ? minVal : (value > maxVal) ? maxVal : value;
    }

    OurRGB bilerp(const Vector& xy) {
    uint32_t w = envMap->w;
    const std::vector<OurRGB>& data = envMap->data;
    double x = xy.X, y = xy.Y;
    OurRGB ret;
    for (int i = 0; i < 4; ++i) {
        double weightX = (i % 2) ? (x - std::floor(x)) : (std::ceil(x) - x);
        double weightY = (i / 2) ? (y - std::floor(y)) : (std::ceil(y) - y);
        int indexX = clamp(static_cast<int>(std::floor(x)) + (i % 2), 0, static_cast<int>(w - 1));
        int indexY = clamp(static_cast<int>(std::floor(y)) + (i / 2), 0, static_cast<int>(envMap->h - 1));
        ret += weightX * weightY * data[indexY * w + indexX];
    }
    return ret;
}


    Vector dir_to_theta_phi(Vector &dir) {
        // Convert direction vector to spherical coordinates (theta, phi)
        // theta: inclination angle (0 to pi)
        // phi: azimuthal angle (0 to 2*pi)
        // dir.unit();
        float theta = acos(dir.Y);
        float phi = atan2(dir.X, dir.Z);
        return Vector(theta, phi, 0.f);
    }


    Vector theta_phi_to_xy(float theta, float phi) {
        // Convert spherical coordinates (theta, phi) to Cartesian coordinates (x, y)
        // x and y are coordinates on the light's surface

        uint32_t w = envMap->w, h = envMap->h;

        float u = (phi + M_PI / 2.0f) / M_PI; // Map phi to range [0, 1]
        float v = theta / M_PI;      // Map theta to range [0, 1]

        float x = u * w; // Map u to range [0, width-1]
        float y = v * h; // Map v to range [0, height-1]

        return Vector(x, y, 0);
    }

   
    //global illumination compendium uniform sampling sphere
    OurRGB Sample_L (float *r, Point *p, float &_pdf) {
        
        // p->X = worldCenter.X + 2 * worldRadius * cos(2 * M_PI * r[0]) * sqrt(r[1] * (1 - r[1]));
        // p->Y = worldCenter.Y + 2 * worldRadius * sin(2 * M_PI * r[0]) * sqrt(r[1] * (1 - r[1]));
        // p->Z = worldCenter.Z + worldRadius * (1 -2 * r[1]);
        
        
        double z = r[0];
        double sinTheta = sqrt(std::max(0.0, 1.0f - z * z));

        double phi = 2.0f * M_PI * r[1];
        Vector x = Vector(cos(phi) * sinTheta, sin(phi) * sinTheta, z);

        p->X = x.X;
        p->Y = x.Y;
        p->Z = x.Z;

        // Vector zz(p->X, p->Y, p->Z);
        // zz.normalize();
        // float thetaz = 2 * M_PI * r[0];
        // float phiz = acos(1-2*r[1]);
        float theta = dir_to_theta_phi(x).X;
        float phiz = dir_to_theta_phi(x).Y;
        Vector xy = theta_phi_to_xy(theta,phiz);

        //std::cout << this->worldCenter.X <<" "<< this->worldCenter.Y<< " " << std::endl;
        //std::cout << xy.X<<" "<< xy.Y<< " " << std::endl;
        /*
        float theta = 2 * M_PI * r[0];
        float phi = acos(1 -2*r[1]);
        Vector xy = theta_phi_to_xy(theta, phi);
       
        */

        _pdf = pdf;

        return bilerp(xy);
    }
};

#endif 