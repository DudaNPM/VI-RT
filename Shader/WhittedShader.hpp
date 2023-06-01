//
//  AmbientShader.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//


#ifndef WhittedShader_hpp
#define WhittedShader_hpp

#include "shader.hpp"


class WhittedShader: public Shader {
    OurRGB background;
    OurRGB directLighting (Intersection isect, Phong *f);
    OurRGB specularReflection (Intersection isect, Phong *f);
public:
    WhittedShader (Scene *scene, OurRGB bg): background(bg), Shader(scene) {}
    OurRGB shade (bool intersected, Intersection isect, int depth);
};

#endif /* AmbientShader_hpp */
