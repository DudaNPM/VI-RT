//
//  AABB.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include "BB.hpp"

#include <utility>
#include <algorithm>

using namespace std;

// Algorithm explained: https://www.youtube.com/watch?v=USjbg5QXk3g
bool BB::intersect (Ray ray) {
    // f_low e f_high são as frações calculadas até agora
    float f_low = 0;
    float f_high = 1;

    // frações da dimensão X/Y/Z em que estivermos a trabalhar
    float curr_f_low, curr_f_high;


    // X axis //
    curr_f_low = (this->min.X - ray.o.X) / ray.dir.X;
    curr_f_high = (this->max.X - ray.o.X) / ray.dir.X;

    if (curr_f_high < curr_f_low)
        swap(curr_f_high, curr_f_low);
    
    if (curr_f_high < f_low || curr_f_low > f_high)
        return false;

    f_low = std::max(curr_f_low, f_low);
    f_high = std::min(curr_f_high, f_high);

    if (f_low > f_high)
        return false;
    

    // Y axis //
    curr_f_low = (this->min.Y - ray.o.Y) / ray.dir.Y;
    curr_f_high = (this->max.Y - ray.o.Y) / ray.dir.Y;

    if (curr_f_high < curr_f_low)
        swap(curr_f_high, curr_f_low);
    
    if (curr_f_high < f_low || curr_f_low > f_high)
        return false;

    f_low = std::max(curr_f_low, f_low);
    f_high = std::min(curr_f_high, f_high);

    if (f_low > f_high)
        return false;

    
    // Z axis //
    curr_f_low = (this->min.Z - ray.o.Z) / ray.dir.Z;
    curr_f_high = (this->max.Z - ray.o.Z) / ray.dir.Z;

    if (curr_f_high < curr_f_low)
        swap(curr_f_high, curr_f_low);
    
    if (curr_f_high < f_low || curr_f_low > f_high)
        return false;

    f_low = std::max(curr_f_low, f_low);
    f_high = std::min(curr_f_high, f_high);

    if (f_low > f_high)
        return false;
    
    // Ponto de interseção :: I = ray.o + (ray.dir * f_low)
    return true;
}