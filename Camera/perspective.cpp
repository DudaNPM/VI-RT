//
//  perspective.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include "perspective.hpp"

bool Perspective::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {
    float xs = (2 * (x + 0.5) / this->W ) - 1;
    float ys = (2 * (y + 0.5) / this->W ) - 1;

    float xc = xs * tanf(this->fovW / 2);
    float yc = ys * tanf(this->fovH / 2);

    r->o = this->Eye;
    r->dir = Vector(this->c2w[0][0] * xc + this->c2w[0][1] * yc + this->c2w[0][2] * 1,
                    this->c2w[1][0] * xc + this->c2w[1][1] * yc + this->c2w[1][2] * 1,
                    this->c2w[2][0] * xc + this->c2w[2][1] * yc + this->c2w[2][2] * 1);

    return true;
}
