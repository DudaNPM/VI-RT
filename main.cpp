//
//  main.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include "scene.hpp"
#include "perspective.hpp"
#include "renderer.hpp"

int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective cam(640, 480);  // image resolution
    bool success;
    
    // success = scene.Load("C:/Users/uncha/Documents/Masters/VI/Project/VI-RT/Scene/tinyobjloader/models/cornell_box.obj");
    success = scene.Load("C:/Users/duart/Desktop/VI/VI-RT/Scene/tinyobjloader/models/cornell_box.obj");
    // scene.print();

    Vector dir (0,10.0,0);
    Point origin (499.0,-10.0,10.0);
    Ray ray (origin,dir);
    Intersection intersection;
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }

    std::cout << "Scene Load: SUCCESS!! :-)\n";

    scene.trace(ray, &intersection);
    
    printf("\nI = x:%f y:%f z:%f\n", intersection.p.X, intersection.p.Y, intersection.p.Z);

    // scene.printSummary();
    std::cout << std::endl;
    
    // declare the renderer
    Renderer myRender (&cam, &scene);
    // render
    myRender.Render();
    // save the image
    cam.SaveImage();
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}
