#include "DistributedShader.hpp"


RGB DistributedShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0.,0.,0.);
    
    // if no intersection, return background
    if (!intersected) return background;
    
    // intersection with a light source
    if (isect.isLight) return isect.power;

    Phong *f = static_cast<Phong *> (isect.f);
    
    // if there is a specular component sample it
    if (!f->Ks.isZero()) color += specularReflection(isect,f);
    
    // if there is a diffuse component do direct light
    if (!f->Kd.isZero()) color += directLightingMonteCarlo(isect,f);
    
    return color;
}


RGB DistributedShader::directLighting(Intersection isect, Phong *f) {
    RGB color(0.,0.,0.);
    
    for (auto& light: scene->lights) {

        if (light->type == AMBIENT_LIGHT) { // is it an ambient light ?
            if (!f->Ka.isZero()) color += f->Ka * light->L();
            continue;
        }

        if (light->type == POINT_LIGHT) { // is it a point light ?
            // ...
        }
        
        if (light->type == AREA_LIGHT) { // is it an area light ?
            if (!f->Kd.isZero()) {
                RGB L;
                Point lpoint;
                float l_pdf;
                auto al = static_cast<AreaLight *> (light);

                float rnd[2];
                rnd[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                rnd[1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                L = al->Sample_L(rnd, &lpoint, l_pdf);
                
                // compute the direction from the intersection point to the light source
                Vector Ldir = isect.p.vec2point(lpoint);
                const float Ldistance = Ldir.norm();
                Ldir.normalize();

                // cosine between Ldir and the shading normal at the intersection point
                float cosL = Ldir.dot(isect.sn);
                // cosine between Ldir and the area light source normal
                float cosL_LA = Ldir.dot(al->gem->normal);
                
                // shade
                if (cosL > 0.0 && cosL_LA <= 0.0) { // light NOT behind primitive AND light normal points to the ray o
                    Ray shadow(isect.p, Ldir); // generate the shadow ray
                    shadow.adjustOrigin(isect.gn); // adjust origin EPSILON along the normal: avoid self oclusion
                    
                    if (scene->visibility(shadow, Ldistance-EPSILON)) { // light source not occluded
                        color += f->Kd * L * cosL / l_pdf;
                    }
                } // end cosL > 0.
            }
        } // end area light
    }
    return color;
}


RGB DistributedShader::directLightingMonteCarlo(Intersection isect, Phong *f) {
    RGB color(0.,0.,0.);

    int n = scene->lights.size();
    int indx = rand() % n;
    auto &light = scene->lights.at(indx);

    if (light->type == AMBIENT_LIGHT) { // is it an ambient light ?
        if (!f->Ka.isZero()) {
                RGB Ka = f->Ka;
                color = Ka * light->L();
        }
    }

    else if (light->type == POINT_LIGHT) { // is it a point light ?
        // ...
    }

    else if (light->type == AREA_LIGHT) { // is it an area light ?
        if (!f->Kd.isZero()) {
            RGB L;
            Point lpoint;
            float l_pdf;
            auto al = static_cast<AreaLight *> (light);
            float rnd[2];
            rnd[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            rnd[1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            L = al->Sample_L(rnd, &lpoint, l_pdf);
            
            // compute the direction from the intersection point to the light source
            Vector Ldir = isect.p.vec2point(lpoint);
            const float Ldistance = Ldir.norm();
            Ldir.normalize();
            // cosine between Ldir and the shading normal at the intersection point
            float cosL = Ldir.dot(isect.sn);
            // cosine between Ldir and the area light source normal
            float cosL_LA = Ldir.dot(al->gem->normal);
            
            // shade
            if (cosL > 0.0 && cosL_LA <= 0.0) { // light NOT behind primitive AND light normal points to the ray o
                Ray shadow(isect.p, Ldir); // generate the shadow ray
                shadow.adjustOrigin(isect.gn); // adjust origin EPSILON along the normal: avoid self oclusion
                
                if (scene->visibility(shadow, Ldistance-EPSILON)) { // light source not occluded
                    color = (f->Kd * L * cosL / l_pdf) * n;
                }
            } // end cosL > 0.
        }
    } // end area light

    return color;
}


RGB DistributedShader::specularReflection(Intersection isect, Phong *f) {
    // generate the specular ray
    float cos = isect.gn.dot(isect.wo);
    Vector Rdir = 2.f * cos * isect.gn - isect.wo;
    Ray specular(isect.p, Rdir);
    specular.adjustOrigin(isect.gn);
    
    // trace ray
    Intersection s_isect;
    bool intersected = scene->trace(specular, &s_isect);
    
    // shade this intersection
    RGB color = shade(intersected, s_isect, 0);
    return color;
}