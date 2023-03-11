//
//  camera.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef camera_hpp
#define camera_hpp

#include "ray.hpp"

// based on pbrt book, sec 6.1, pag. 356
class Camera {
public:
    Camera () {}
    ~Camera() {}
    //bool GenerateRay() {return false;};
    bool virtual GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter=NULL) {return false;};
    virtual void getResolution (int *_W, int *_H) {};
};

#endif /* camera_hpp */
