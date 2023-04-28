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
#include "WhittedShader.hpp"
#include "DistributedShader.hpp"
#include "AmbientLight.hpp"
#include "PointLight.hpp"
#include "AreaLight.hpp"

int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *img;    // Image
    Shader *shd;
    bool success;
    
    // success = scene.Load("C:/Users/uncha/Documents/Masters/VI/Project/VI-RT/Scene/tinyobjloader/models/cube.obj");
    success = scene.Load("C:/Users/duart/Desktop/VI/VI-RT/Scene/tinyobjloader/models/cornell_box_VI.obj");
    // success = scene.Load("C:/Users/User/Desktop/CG/VI/VI-RT/Scene/tinyobjloader/models/cornell_box.obj");
    // scene.print();
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }

    std::cout << "Scene Load: SUCCESS!! :-)\n";

    // add an ambient light to the scene
    AmbientLight *ambient = new AmbientLight(RGB(0.2f,0.2f,0.2f));
    scene.lights.push_back(ambient);
    scene.numLights++;

    // add a point light to the scene
    // PointLight *pl1 = new PointLight(RGB(0.65f,0.65f,0.65f),Point(288,508,282));
    // scene.lights.push_back(pl1);
    // scene.numLights++;

    // add a area light to the scene
    AreaLight *al1 = new AreaLight(RGB(1.,1.,1.),Point(300,508,282),Point(280,508,282),Point(280,508,315),Vector(0,-1,0));
    scene.lights.push_back(al1);
    scene.numLights++;


    scene.printSummary();
    
    // Image resolution
    const int W = 1024;
    const int H = 1024;

    img = new ImagePPM(W,H);

    // Camera parameters
    const Point Eye = {280,275,-330};
    const Point At = {280,265,0};
    const Vector Up = {0,1,0};
    const float fovW = 3.1415f / 2.0f;
    const float fovH = fovW * H/W;
    cam = new Perspective(Eye, At, Up, W, H, fovW, fovH);
    
    // create the shader
    RGB background(0.05f, 0.05f, 0.55f);
    // shd = new WhittedShader(&scene, background);
    shd = new DistributedShader(&scene, background);
    // declare the renderer
    StandardRenderer myRender (cam, &scene, img, shd);
    // render
    myRender.Render();
    // save the image
    img->Save("MyImage.ppm");
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}
