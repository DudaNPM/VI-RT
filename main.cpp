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
    
    // success = scene.Load("C:/Users/uncha/Documents/Masters/VI/Project/VI-RT/Scene/tinyobjloader/models/cornell_box_VI.obj");
    success = scene.Load("C:/Users/duart/Desktop/VI/VI-RT/Scene/tinyobjloader/models/cornell_box_VI.obj");
    // scene.print();
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }

    std::cout << "Scene Load: SUCCESS!! :-)\n";

    // add an ambient light to the scene
    AmbientLight *ambient = new AmbientLight(RGB());
    scene.lights.push_back(ambient);
    scene.numLights++;

    // add a point light to the scene
    // PointLight *pl1 = new PointLight(RGB(0.65f,0.65f,0.65f),Point(288,508,282));
    // scene.lights.push_back(pl1);
    // scene.numLights++;

    // add a area light to the scene
    Vector n(0,-1,0);
    AreaLight *al01 = new AreaLight(RGB(0.4f,0.4f,0.4f),Point(069.50f,548.80f,489.30f),Point(069.50f,548.80f,419.40f),Point(139.00f,548.80f,419.40f),n);
    AreaLight *al02 = new AreaLight(RGB(0.4f,0.4f,0.4f),Point(069.50f,548.80f,489.30f),Point(139.00f,548.80f,419.40f),Point(139.00f,548.80f,489.30f),n);
    AreaLight *al03 = new AreaLight(RGB(0.4f,0.4f,0.4f),Point(139.00f,548.70f,139.80f),Point(069.50f,548.75f,139.80f),Point(139.00f,548.65f,069.90f),n);
    AreaLight *al04 = new AreaLight(RGB(0.4f,0.4f,0.4f),Point(139.00f,548.65f,069.90f),Point(069.50f,548.75f,139.80f),Point(069.50f,548.73f,069.90f),n);
    AreaLight *al05 = new AreaLight(RGB(0.4f,0.4f,0.4f),Point(417.00f,548.40f,139.80f),Point(417.00f,548.30f,069.90f),Point(486.50f,548.20f,069.90f),n);
    AreaLight *al06 = new AreaLight(RGB(0.4f,0.4f,0.4f),Point(486.50f,548.20f,069.90f),Point(486.50f,548.30f,139.80f),Point(417.00f,548.40f,139.80f),n);
    AreaLight *al07 = new AreaLight(RGB(0.4f,0.4f,0.4f),Point(417.00f,548.75f,489.30f),Point(417.00f,548.70f,419.40f),Point(486.50f,548.65f,419.40f),n);
    AreaLight *al08 = new AreaLight(RGB(0.4f,0.4f,0.4f),Point(486.50f,548.65f,419.40f),Point(417.00f,548.75f,489.30f),Point(486.50f,548.73f,489.30f),n);
    AreaLight *al09 = new AreaLight(RGB(1.0f,1.0f,1.0f),Point(243.25f,548.80f,314.55f),Point(243.25f,548.76f,244.65f),Point(312.75f,548.70f,244.65f),n);
    AreaLight *al10 = new AreaLight(RGB(1.0f,1.0f,1.0f),Point(312.75f,548.70f,244.65f),Point(312.75f,548.76f,314.75f),Point(243.25f,548.80f,314.55f),n);
    scene.lights.push_back(al01);
    scene.lights.push_back(al02);
    scene.lights.push_back(al03);
    scene.lights.push_back(al04);
    scene.lights.push_back(al05);
    scene.lights.push_back(al06);
    scene.lights.push_back(al07);
    scene.lights.push_back(al08);
    scene.lights.push_back(al09);
    scene.lights.push_back(al10);
    scene.numLights += 10;


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
