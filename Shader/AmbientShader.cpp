//
//  AmbientShader.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "AmbientShader.hpp"
#include "Phong.hpp"

RGB AmbientShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0.,0.,0.);
    
    // if no intersection, return background
    if (!intersected) return this->background;

    // verify whether the intersected object has an ambient component
    auto f = static_cast<Phong *> (isect.f);
    if (f->Ka.isZero()) return color;
    RGB Ka = f->Ka;

    // ambient shade
    // Loop over scene's light sources and process Ambient Lights
    for (auto& light : scene->lights) {
        // is it an ambient light ?
        if (light->type == AMBIENT_LIGHT)
            color += Ka * light->L();
    }

    return color;
};
