#ifndef PathTracerShader_hpp
#define PathTracerShader_hpp


#include "shader.hpp"
#include "InfiniteAreaLight.hpp"
#include "vector.hpp"


class PathTracerShader: public Shader {
    OurRGB background;
    OurRGB directLighting (Intersection isect, Phong *f);
    OurRGB directLightingMonteCarlo(Intersection isect, Phong *f);
    OurRGB specularReflection (Intersection isect, Phong *f, int depth);
    OurRGB diffuseReflection (Intersection isect, Phong *f, int depth);
    float continue_p;
    int MAX_DEPTH;
public:
    PathTracerShader (Scene *scene, OurRGB bg): background(bg),Shader(scene) {continue_p = 0.5f; MAX_DEPTH=1;}
    OurRGB shade (bool intersected, Intersection isect, int depth, Ray ray);
};

#endif /* PathTracerShader_hpp */