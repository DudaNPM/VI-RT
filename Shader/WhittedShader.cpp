//
//  AmbientShader.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "WhittedShader.hpp"
#include "Phong.hpp"
#include "ray.hpp"

RGB WhittedShader::directLighting (Intersection isect, Phong *f) {
    RGB color(0.,0.,0.);
    
    // Loop over scene's light sources
    for (auto& light : scene->lights) {
        
        if (light->type == AMBIENT_LIGHT) {  // is it an ambient light ?
            if (!f->Ka.isZero()) {
                RGB Ka = f->Ka;
                color += Ka * light->L();
            }
            continue;
        }
        
        if (light->type == POINT_LIGHT) {  // is it a point light ?
            if (!f->Kd.isZero()) {
                Point lpoint;

                // get the position and radiance of the light source
                RGB L = light->Sample_L(NULL, &lpoint);

                // compute the direction from the intersection to the light
                Vector Ldir = isect.p.vec2point(lpoint);
                const float Ldistance = Ldir.norm();
                Ldir.normalize();

                // compute the cosine (Ldir, shadding normal)
                float cosL = Ldir.dot(isect.gn);
                if (cosL > 0.0) { // the light is NOT behind the primitive
                    // generate the shadow ray
                    Ray shadow(isect.p, Ldir);
                    // adjust origin EPSILON along the normal: avoid self oclusion
                    shadow.adjustOrigin(isect.gn);

                    if (scene->visibility(shadow, Ldistance-EPSILON)) { // light source not occlude
                        color += f->Kd * L * RGB(cosL, cosL, cosL); // ERROR: RGB * float
                    } // end cosL > 0
                }
            }
        }
    }
    return color;
}

RGB WhittedShader::specularReflection (Intersection isect, Phong *f) {
    // generate the specular ray
    float cos = isect.gn.dot(isect.wo);
    Vector Rdir = 2.f * cos * isect.gn - isect.wo;
    Ray specular(isect.p, Rdir);
    specular.adjustOrigin(isect.gn);
    
    // trace ray
    Intersection s_isect;
    bool intersected = scene->trace(specular, &s_isect);
    
    // shade this intersection
    RGB color = shade(intersected, s_isect);
    return color;
}

RGB WhittedShader::shade(bool intersected, Intersection isect) {
    RGB color(0.,0.,0.);
    
    // if no intersection, return background
    if (!intersected) { return (background); }
    
    // get the BRDF
    Phong *f = static_cast<Phong *> (isect.f);
    
    // if there is a specular component sample it
    if (!f->Ks.isZero())
        color += specularReflection (isect, f);
    
    color += directLighting(isect, f);
        
    return color;
};
