//
//  shader.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//


#ifndef shader_hpp
#define shader_hpp

#define _USE_MATH_DEFINES

#include "AreaLight.hpp"
#include "scene.hpp"
#include "Phong.hpp"
#include "light.hpp"
#include "RGB.hpp"
#include "ray.hpp"


class Shader {
protected:
    Scene *scene;
public:
    Shader (Scene *_scene): scene(_scene) {}
    ~Shader () {}
    virtual RGB shade (bool intersected, Intersection isect, int depth) {return RGB();}
};

#endif /* shader_hpp */
