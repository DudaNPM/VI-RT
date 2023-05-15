#include "PathTracerShader.hpp"


RGB PathTracerShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0.,0.,0.);
    
    // if no intersection, return background
    if (!intersected) return background;
    
    // intersection with a light source
    if (isect.isLight) return isect.power;
    
    // get the BRDF
    Phong *f = static_cast<Phong *> (isect.f);

    float rnd_russian = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    if (depth < MAX_DEPTH || rnd_russian < continue_p) {
        RGB lcolor(0.,0.,0.);
        
        // random select between specular and diffuse
        float s_p = f->Ks.Y() / (f->Ks.Y() + f->Kd.Y());
        float rnd = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        // do specular
        if (rnd < s_p) {
            if (!f->Ks.isZero())
                lcolor = specularReflection(isect, f, depth+1) / s_p;
        }
        // do diffuse
        else {
            if (!f->Kd.isZero())
                lcolor = diffuseReflection(isect, f, depth+1) / (1.0f - s_p);
        }
        
        // no russian roulette
        if (depth < MAX_DEPTH) color += lcolor;
        // russian roulette
        else color += lcolor / continue_p;
    }
    
    // if there is a diffuse component do direct light
    if (!f->Kd.isZero()) color += directLightingMonteCarlo(isect,f);
    
    return color;
}


RGB PathTracerShader::directLighting(Intersection isect, Phong *f) {
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


RGB PathTracerShader::directLightingMonteCarlo(Intersection isect, Phong *f) {
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
                    color = (f->Kd * L * cosL / l_pdf) * (float) n;
                }
            } // end cosL > 0.
        }
    } // end area light

    return color;
}


RGB PathTracerShader::specularReflection(Intersection isect, Phong *f, int depth) {
    RGB color(0.,0.,0.);
    Vector s_dir;
    float pdf;
    Intersection s_isect;

    // generate the specular ray
    float cos = isect.gn.dot(isect.wo);
    Vector Rdir = 2.f * cos * isect.gn - isect.wo;

    if (f->Ns >= 1000) { // ideal specular
        Ray specular(isect.p, Rdir);
        specular.adjustOrigin(isect.gn);
    
        // trace ray
        bool intersected = scene->trace(specular, &s_isect);
        
        // shade this intersection
        RGB Rcolor = shade(intersected, s_isect, depth+1);
        color = f->Ks * Rcolor;
        return color;
    }
    
    else { // glossy materials
        float rnd[2];
        rnd[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        rnd[1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        
        Vector S_around_N;
        
        const float cos_theta = powf(rnd[1], 1.0f/(f->Ns+1.0f));
        S_around_N.Z = cos_theta;
        
        const float aux_r1 = powf(rnd[1], 2.f/(f->Ns+1.f));
        S_around_N.Y = sinf(2.f*(float)M_PI*rnd[0]) * sqrtf(1.f-aux_r1);
        S_around_N.X = cosf(2.f*(float)M_PI*rnd[0]) * sqrtf(1.f-aux_r1);
        
        pdf = (f->Ns+1.f) * powf(cos_theta, f->Ns) / (2.f*(float)M_PI);
        
        Vector Rx, Ry;
        Rdir.CoordinateSystem(&Rx, &Ry);
        s_dir = S_around_N.Rotate (Rx, Ry, Rdir);
        
        Ray specular(isect.p, s_dir);
        specular.adjustOrigin(isect.gn);
        bool intersected = scene->trace(specular, &s_isect);
        
        RGB Rcolor = shade(intersected, s_isect, depth+1);
        color = (f->Ks * Rcolor * powf(cos_theta, f->Ns)/(2.f*(float)M_PI)) / pdf;
        return color;
    }
}



RGB PathTracerShader::diffuseReflection(Intersection isect, Phong *f, int depth) {
    RGB color(0.,0.,0.); Vector dir; float pdf;

    // actual direction distributed around N: 2 random number in [0,1[
    float rnd[2];
    rnd[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    rnd[1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    
    Vector D_around_Z;
    float cos_theta = D_around_Z.Z = sqrtf(rnd[1]); // cos sampling
    D_around_Z.Y = sinf(2.f*(float)M_PI*rnd[0])*sqrtf(1.0f-rnd[1]);
    D_around_Z.X = cosf(2.f*(float)M_PI*rnd[0])*sqrtf(1.0f-rnd[1]);
    pdf = cos_theta / (float)M_PI;
    
    // generate a coordinate system from N
    Vector Rx, Ry;
    isect.gn.CoordinateSystem(&Rx,&Ry);
    Ray diffuse(isect.p, D_around_Z.Rotate(Rx, Ry, isect.gn));
    diffuse.adjustOrigin(isect.gn);
    
    // OK, we have the ray : trace and shade it recursively
    bool intersected = scene->trace(diffuse, &isect);
    
    // if light source return 0 ; handled by direct
    if (!isect.isLight) { // shade this intersection
        RGB Rcolor = shade(intersected, isect, depth+1);
        color = (f->Kd * cos_theta * Rcolor) / pdf;
    }

    return color;
}