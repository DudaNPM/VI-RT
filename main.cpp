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
#include "StandardRenderer.hpp"
#include "ImagePPM.hpp"
#include "AmbientShader.hpp"
#include "AmbientLight.hpp"

int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *img;    // Image
    Shader *shd;
    bool success;
    
    // success = scene.Load("C:/Users/uncha/Documents/Masters/VI/Project/VI-RT/Scene/tinyobjloader/models/cornell_box.obj");
    success = scene.Load("C:/Users/duart/Desktop/VI/VI-RT/Scene/tinyobjloader/models/cornell_box.obj");
    // success = scene.Load("C:/Users/User/Desktop/CG/VI/VI-RT/Scene/tinyobjloader/models/cornell_box.obj");
    // scene.print();
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }

    std::cout << "Scene Load: SUCCESS!! :-)\n";
    scene.printSummary();
    // std::cout << std::endl;

    // add an ambient light to the scene
    AmbientLight ambient(RGB(0.9f,0.9f,0.9f));
    scene.lights.push_back(ambient);
    scene.numLights++;
    
    // Image resolution
    const int W = 640;
    const int H = 480;

    img = new ImagePPM(W,H);

    // Camera parameters
    const Point Eye = {0,1000,0};
    const Point At = {0,0,0};
    const Vector Up = {0,1,0};
    const float fovW = 3.14f/3.f;
    const float fovH = 3.14f/3.f;
    cam = new Perspective(Eye, At, Up, W, H, fovW, fovH);
    
    // create the shader
    RGB background(0.05f, 0.05f, 0.55f);
    shd = new AmbientShader(&scene, background);
    // declare the renderer
    StandardRenderer myRender (cam, &scene, img, shd);
    // render
    myRender.Render();
    // save the image
    img->Save("MyImage.ppm");
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}
