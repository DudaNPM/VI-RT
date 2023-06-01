#ifndef InfiniteAreaLight_hpp
#define InfiniteAreaLight_hpp


#include "light.hpp"
#include "vector.hpp"
#include "EnvMap.hpp"
#include <iostream>

#include <math.h>

class InfiniteAreaLight: public Light {
public:
    const HDRImageBuffer* envMap;
    Point worldCenter;
    float worldRadius;

    float pdf;

    InfiniteAreaLight(HDRImageBuffer* envMap, Point worldCenter, float worldRadius){
        type = INFINITE_LIGHT;
        pdf = 1.0f / (4 * M_PI );// for uniform sampling the 
        this->worldRadius = worldRadius;
        this->worldCenter = worldCenter;
        this->envMap = envMap;
    }
    
    ~InfiniteAreaLight() {delete envMap;}
    

    OurRGB bilerp(const Vector& xy) const {
	    uint32_t w = envMap->w;
	    const std::vector<OurRGB>& data = envMap->data;
	    double x = xy.X, y = xy.Y;
        //std::cout << "X " << x << " Y "<< y << " "  << std::endl;
        //std::cout << "W " << w << " H "<< envMap->h << " "  << std::endl;
	    OurRGB ret;
	    for (int i = 0; i < 4; ++i){
		    ret += (i%2 ? x-floor(x) : ceil(x)-x) * 
			    (i/2 ? y-floor(y) : ceil(y)-y) * 
			    data[(w * (floor(y) + i/2) + floor(x) + i%2)];
        }
        //std::cout << "LIGHT " << ret.R << " "<< ret.G << " " << ret.B << std::endl;
	    return ret;
    }
    Vector theta_phi_to_xy(float theta, float phi) const {
        uint32_t w = envMap->w, h = envMap->h;
        double x = phi / 2. / M_PI * w;
        double y = theta / M_PI * h;
        //std::cout << x << " " << y << std::endl;
        return Vector(x, y, 0);
    }

    Vector dir_to_theta_phi(Vector &dir){
        dir.unit();
        float theta = acos(dir.Y);
        float phi = atan2(dir.Z, dir.X);
        return Vector(theta, phi,0.f);
    }

   
    //global illumination compendium uniform sampling sphere
    OurRGB Sample_L (float *r, Point *p, float &_pdf) {
        
        p->X = worldCenter.X + 2 * worldRadius * cos(2 * M_PI * r[0]) * sqrt(r[1] * (1 - r[1]));
        p->Y = worldCenter.Y + 2 * worldRadius * sin(2 * M_PI * r[0]) * sqrt(r[1] * (1 - r[1]));
        p->Z = worldCenter.Z + worldRadius * (1 -2 * r[1]);
        /*
        double z = r[0];
        double sinTheta = sqrt(std::max(0.0, 1.0f - z * z));

        double phi = 2.0f * M_PI * r[1];
        Vector x = Vector(cos(phi) * sinTheta, sin(phi) * sinTheta, z);

        */
        Vector zz(p->X, p->Y, p->Z);
        //double thetaz = 2 * M_PI * r[0];
        //double phiz = acos(1-2*r[1]);
        //Vector zz(thetaz, phiz, 0);
        float theta = zz.vec_to_SphericalTheta();
        float phi = zz.vec_to_SphericalPhi();
        Vector xy = theta_phi_to_xy(theta,phi);

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