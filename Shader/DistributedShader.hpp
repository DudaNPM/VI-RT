#ifndef DistributedShader_hpp
#define DistributedShader_hpp


#include "shader.hpp"


class DistributedShader: public Shader {
    OurRGB background;
    OurRGB directLighting (Intersection isect, Phong *f);
    OurRGB directLightingMonteCarlo(Intersection isect, Phong *f);
    OurRGB specularReflection (Intersection isect, Phong *f);
public:
    DistributedShader (Scene *scene, OurRGB bg): background(bg),Shader(scene) {}
    OurRGB shade (bool intersected, Intersection isect, int depth);
};

#endif /* DistributedShader_hpp */
