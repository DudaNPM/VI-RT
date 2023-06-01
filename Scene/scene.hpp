//
//  Scene.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <iostream>
#include <string>
#include <vector>
#include "primitive.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "intersection.hpp"
#include "BRDF.hpp"

#include <iostream>
#include <random>


class Scene {
private:

    std::vector<Vector> normals;
    std::vector <Primitive> prims;
    std::vector <std::shared_ptr<BRDF>> BRDFs;
public:
    std::vector <Light *> lights;
    std::vector<Point> vertices;
    int numPrimitives, numLights, numBRDFs;
    
    Scene (): numPrimitives(0), numLights(0), numBRDFs(0) {}
    bool Load (const std::string &fname);
    bool SetLights (void) { return true; };
    bool trace (Ray r, Intersection *isect);
    bool visibility (Ray s, const float maxL);
    void printSummary(void) {
        std::cout << "#primitives = " << numPrimitives << " ; ";
        std::cout << "#lights = " << numLights << " ; ";
        std::cout << "#materials = " << numBRDFs << " ;" << std::endl;
    }
    void print();
};

#endif /* Scene_hpp */
