//
//  renderer.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//


#ifndef renderer_hpp
#define renderer_hpp

#define _CRT_RAND_S

#include "shader.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "image.hpp"

#include <stdlib.h>
#include <thread>
#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std::chrono;


class Renderer {
public:
    Scene *scene;
    Image * img;
    Shader *shd;
    Camera *cam;
    Renderer (Camera *cam, Scene * scene, Image * img, Shader *shd): cam(cam), scene(scene), img(img), shd(shd) {}
    virtual void Render () {}
};

#endif /* renderer_hpp */
