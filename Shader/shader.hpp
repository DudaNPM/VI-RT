//
//  shader.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//


#ifndef shader_hpp
#define shader_hpp

#define _USE_MATH_DEFINES

#include "InfiniteAreaLight.hpp"
#include "AreaLight.hpp"
#include "scene.hpp"
#include "Phong.hpp"
#include "light.hpp"
#include "RGB.hpp"
#include "ray.hpp"
#include "vector.hpp"


class Shader {
protected:
    Scene *scene;
public:
    Shader (Scene *_scene): scene(_scene) {}
    ~Shader () {}
    virtual OurRGB shade (bool intersected, Intersection isect, int depth,Ray ray) {return OurRGB();}
};

#endif /* shader_hpp */
