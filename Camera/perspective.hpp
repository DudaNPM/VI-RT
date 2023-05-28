//
//  perspective.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef perspective_hpp
#define perspective_hpp


#include "camera.hpp"
#include "vector.hpp"
#include "ray.hpp"

#include <iostream>


class Perspective: public Camera {
    Point Eye, At;
    Vector Up;
    float fovW, fovH;
    int W, H;
    float c2w[3][3];  // camera 2 world transform
public:
    Perspective (const Point Eye, const Point At, Vector Up, const int W, const int H, const float fovW, const float fovH): Eye(Eye), At(At), Up(Up), W(W), H(H), fovW(fovW), fovH(fovH)  {
        // compute camera 2 world transform
        Vector F (At.X-Eye.X,At.Y-Eye.Y,At.Z-Eye.Z);
        F.normalize();
        
        Vector R = F.cross(Up);
        R.normalize();

        c2w[0][0] = R.X;
        c2w[0][1] = R.Y;
        c2w[0][2] = R.Z;

        c2w[1][0] = Up.X;
        c2w[1][1] = Up.Y;
        c2w[1][2] = Up.Z;

        c2w[2][0] = F.X;
        c2w[2][1] = F.Y;
        c2w[2][2] = F.Z;
    }
    bool GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter=NULL);
    void getResolution (int *_W, int *_H) {*_W=W; *_H=H;}
};

#endif /* perspective_hpp */
