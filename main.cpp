//
//  main.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//


#include "DistributedShader.hpp"
#include "PathTracerShader.hpp"
#include "StandardRenderer.hpp"
#include "AmbientShader.hpp"
#include "WhittedShader.hpp"
#include "AmbientLight.hpp"
#include "perspective.hpp"
#include "PointLight.hpp"
#include "AreaLight.hpp"
#include "renderer.hpp"
#include "ImagePPM.hpp"
#include "scene.hpp"

#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define NUM_THREADS 6
#define SPP 32


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
        cout << "ERROR!! :o\n";
        return 1;
    }
    
    std::cout << "Scene Load: SUCCESS!! :-)\n";

    // add an ambient light to the scene
    AmbientLight *ambient = new AmbientLight(RGB());
    scene.lights.push_back(ambient); scene.numLights++;

    // add a point light to the scene
    // PointLight *pl1 = new PointLight(RGB(0.65f,0.65f,0.65f),Point(288,508,282));
    // scene.lights.push_back(pl1);
    // scene.numLights++;

    // add a area light to the scene
    Vector n(0,-1,0);
    /*
    AreaLight *al01 = new AreaLight(RGB(0.1f,0.1f,0.1f),Point(069.50f,547.80f,489.30f),Point(069.50f,547.80f,419.40f),Point(139.00f,547.80f,419.40f),n);
    AreaLight *al02 = new AreaLight(RGB(0.1f,0.1f,0.1f),Point(069.50f,547.80f,489.30f),Point(139.00f,547.80f,419.40f),Point(139.00f,547.80f,489.30f),n);
    AreaLight *al03 = new AreaLight(RGB(0.1f,0.1f,0.1f),Point(139.00f,547.70f,139.80f),Point(069.50f,547.75f,139.80f),Point(139.00f,547.65f,069.90f),n);
    AreaLight *al04 = new AreaLight(RGB(0.1f,0.1f,0.1f),Point(139.00f,547.65f,069.90f),Point(069.50f,547.75f,139.80f),Point(069.50f,547.73f,069.90f),n);
    AreaLight *al05 = new AreaLight(RGB(0.1f,0.1f,0.1f),Point(417.00f,547.40f,139.80f),Point(417.00f,547.30f,069.90f),Point(486.50f,547.20f,069.90f),n);
    AreaLight *al06 = new AreaLight(RGB(0.1f,0.1f,0.1f),Point(486.50f,547.20f,069.90f),Point(486.50f,547.30f,139.80f),Point(417.00f,547.40f,139.80f),n);
    AreaLight *al07 = new AreaLight(RGB(0.1f,0.1f,0.1f),Point(417.00f,547.75f,489.30f),Point(417.00f,547.70f,419.40f),Point(486.50f,547.65f,419.40f),n);
    AreaLight *al08 = new AreaLight(RGB(0.1f,0.1f,0.1f),Point(486.50f,547.65f,419.40f),Point(417.00f,547.75f,489.30f),Point(486.50f,547.73f,489.30f),n);
    */
    AreaLight *al09 = new AreaLight(RGB(0.5f,0.5f,0.5f),Point(243.25f,547.80f,314.55f),Point(243.25f,547.76f,244.65f),Point(312.75f,547.70f,244.65f),n);
    AreaLight *al10 = new AreaLight(RGB(0.5f,0.5f,0.5f),Point(312.75f,547.70f,244.65f),Point(312.75f,547.76f,314.75f),Point(243.25f,547.80f,314.55f),n);
    /*
    scene.lights.push_back(al01); scene.numLights++;
    scene.lights.push_back(al02); scene.numLights++;
    scene.lights.push_back(al03); scene.numLights++;
    scene.lights.push_back(al04); scene.numLights++;
    scene.lights.push_back(al05); scene.numLights++;
    scene.lights.push_back(al06); scene.numLights++;
    scene.lights.push_back(al07); scene.numLights++;
    scene.lights.push_back(al08); scene.numLights++;
    */
    scene.lights.push_back(al09); scene.numLights++;
    scene.lights.push_back(al10); scene.numLights++;


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
    shd = new PathTracerShader(&scene, background);
    // declare the renderer
    StandardRenderer myRender(cam, &scene, img, shd);

    // start execution time
    auto start = high_resolution_clock::now();

    // render
    // myRender.Render(SPP);
    // myRender.RenderParallelOpenMP(NUM_THREADS,SPP);
    myRender.RenderParallel(NUM_THREADS,SPP);

    // stop execution time
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<seconds>(stop - start);

    cout << "Execution time: " << duration.count() << " seconds" << endl;

    // save the image
    img->Save("32spp_com_rr.ppm");
    
    cout << "That's all, folks!" << endl;
    return 0;
}
