//
//  perspective.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include "perspective.hpp"

bool Perspective::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {
    float xs = (2.0f * (x + 0.5f) / this->W ) - 1.0f;
    float ys = (2.0f * (this->H - y - 1.0f + 0.5f) / this->H ) - 1.0f;

    float xc = xs * tanf(this->fovW / 2.0f);
    float yc = ys * tanf(this->fovH / 2.0f);

    r->o =  this->Eye;
    r->dir = Vector(this->c2w[0][0] * xc  + this->c2w[0][1] * yc + this->c2w[0][2] *  1 ,
                    this->c2w[1][0] * xc  + this->c2w[1][1] * yc + this->c2w[1][2] *  1 ,
                    this->c2w[2][0] * xc  + this->c2w[2][1] * yc + this->c2w[2][2] *  1);
    r->dir.normalize();

    return true;
}
