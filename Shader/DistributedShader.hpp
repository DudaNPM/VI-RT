#ifndef DistributedShader_hpp
#define DistributedShader_hpp

#include "shader.hpp"
#include "Phong.hpp"
#include "RGB.hpp"
#include "AreaLight.hpp"
#include "light.hpp"

class DistributedShader: public Shader {
    RGB background;
    RGB directLighting (Intersection isect, Phong *f);
    RGB directLightingMonteCarlo(Intersection isect, Phong *f);
    RGB specularReflection (Intersection isect, Phong *f);
public:
    DistributedShader (Scene *scene, RGB bg): background(bg),Shader(scene) {}
    RGB shade (bool intersected, Intersection isect, int depth);
};

#endif /* DistributedShader_hpp */
