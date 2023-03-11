//
//  renderer.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef renderer_hpp
#define renderer_hpp

#include "camera.hpp"
#include "scene.hpp"
#include "image.hpp"
#include <iostream>

class Renderer {
protected:
    Camera *cam;
    Scene *scene;
    Image * img;
public:
    Renderer (Camera *cam, Scene * scene, Image * img): cam(cam), scene(scene), img(img) {}
    void Render () {
        int W, H;
        cam->getResolution(&W, &H);


        Ray ray;

        for (int i = 0; i < W; i++) {
            for (int j = 0; j < H; j++) {
                if (cam->GenerateRay(i, j, &ray))
                    img->set(i, j, this->rad(ray));
            }
        }
    }

    RGB rad (Ray ray) {
        Intersection isect;
        if (scene->trace(ray, &isect))
            return this->shade(ray, isect);

        return RGB(0,0,0);
    }

    RGB shade(Ray ray, Intersection isect) {
        return RGB(1,1,1); // change later
    }
};

#endif /* renderer_hpp */
