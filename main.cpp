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
#include "EnvMap.hpp"
#include "InfiniteAreaLight.hpp"

#include <iostream>
#include <list>
#include <chrono>


using namespace std;
using namespace std::chrono;

#define NUM_THREADS 4
#define SPP 64
#define TINYEXR_IMPLEMENTATION
#include "./tinyexr-release/tinyexr.h"




HDRImageBuffer* load_exr(const char* file_path) {

    // 1. Read EXR version.
    EXRVersion exr_version;

    int ret = ParseEXRVersionFromFile(&exr_version,file_path);
    if (ret != 0) {
        std::cout << "Error parsing EXR version" << std::endl;
        fprintf(stderr, "Invalid EXR file: %s\n", file_path);
        return NULL;
    }

    if (exr_version.multipart) {
    // must be multipart flag is false.
        return NULL;
    }

    
  // 2. Read EXR header
    EXRHeader exr_header;
    InitEXRHeader(&exr_header);

    const char* err = NULL; // or `nullptr` in C++11 or later.
    ret = ParseEXRHeaderFromFile(&exr_header, &exr_version, file_path, &err);
    if (ret != 0) {
        fprintf(stderr, "Parse EXR err: %s\n", err);
        FreeEXRErrorMessage(err); // free's buffer for an error message
        return NULL;
    }
      // // Read HALF channel as FLOAT.
    for (int i = 0; i < exr_header.num_channels; i++) {
        if (exr_header.pixel_types[i] == TINYEXR_PIXELTYPE_HALF) {
            exr_header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;
        }
    }

    EXRImage exr_image;
    InitEXRImage(&exr_image);
    

    ret = LoadEXRImageFromFile(&exr_image, &exr_header, file_path, &err);
    if (ret != 0) {
        fprintf(stderr, "Load EXR err: %s\n", err);
        FreeEXRHeader(&exr_header);
        FreeEXRErrorMessage(err); // free's buffer for an error message
        return NULL;
    }

    // 3. Access image data
    // `exr_image.images` will be filled when EXR is scanline format.
    // `exr_image.tiled` will be filled when EXR is tiled format.

    // 4. Free image dat
  

    HDRImageBuffer* envmap = new HDRImageBuffer();
    envmap->resize(exr_image.width, exr_image.height);
    float* channel_r = (float*) exr_image.images[2];
    float* channel_g = (float*) exr_image.images[1];
    float* channel_b = (float*) exr_image.images[0];
    for (size_t i = 0; i < exr_image.width * exr_image.height; i++) {
        envmap->data[i] = OurRGB(channel_r[i], 
                               channel_g[i], 
                               channel_b[i]);
    }
    FreeEXRImage(&exr_image);
    FreeEXRHeader(&exr_header);

    

  return envmap;
}

inline float distance(const Point& p1, const Point& p2) {
    float dx = p1.X - p2.X;
    float dy = p1.Y - p2.Y;
    float dz = p1.Z - p2.Z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}



// Function to calculate the center and radius of the minimum bounding sphere
void ritterBoundingSphere(const std::vector<Point>& points, Point& center, float& radius) {
    // Find the point with the maximum distance from the first point
    size_t indexMax = 0;
    float maxDistance = 0.0f;
    for (size_t i = 1; i < points.size(); ++i) {
        float dist = distance(points[0], points[i]);
        if (dist > maxDistance) {
            maxDistance = dist;
            indexMax = i;
        }
    }

    // Find the point with the maximum distance from the previous point
    size_t indexMax2 = 0;
    maxDistance = 0.0f;
    for (size_t i = 0; i < points.size(); ++i) {
        if (i != indexMax) {
            float dist = distance(points[indexMax], points[i]);
            if (dist > maxDistance) {
                maxDistance = dist;
                indexMax2 = i;
            }
        }
    }

    // Calculate the initial bounding sphere center
    center.X = (points[indexMax].X + points[indexMax2].X) / 2.0f;
    center.Y = (points[indexMax].Y + points[indexMax2].Y) / 2.0f;
    center.Z = (points[indexMax].Z + points[indexMax2].Z) / 2.0f;

    // Calculate the initial bounding sphere radius
    radius = distance(points[indexMax], points[indexMax2]) / 2.0f;

    // Expand the bounding sphere to include all points
    for (const Point& p : points) {
        float dist = distance(center, p);
        if (dist > radius) {
            float newRadius = (radius + dist) / 2.0f;
            float k = (newRadius - radius) / dist;
            center.X = center.X + k * (p.X - center.X);
            center.Y = center.Y + k * (p.Y - center.Y);
            center.Z = center.Z + k * (p.Z - center.Z);
            radius = newRadius;
        }
    }
}





int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *img;    // Image
    Shader *shd;
    bool success;
    
    success = scene.Load("C:/Users/uncha/Documents/Masters/VI/Project/VI-RT/Scene/tinyobjloader/models/cornell_box_VI.obj");
    //success = scene.Load("C:/Users/duart/Desktop/VI/VI-RT/Scene/tinyobjloader/models/cornell_box_VI.obj");
    // scene.print();
    
    if (!success) {
        cout << "ERROR!! :o\n";
        return 1;
    }
    
    std::cout << "Scene Load: SUCCESS!! :-)\n";

    // add an ambient light to the scene
    AmbientLight *ambient = new AmbientLight(OurRGB(0,0,0));
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
    //AreaLight *al09 = new AreaLight(RGB(0.1f,0.1f,0.1f),Point(243.25f,547.80f,314.55f),Point(243.25f,547.76f,244.65f),Point(312.75f,547.70f,244.65f),n);
    //AreaLight *al10 = new AreaLight(RGB(0.1f,0.1f,0.1f),Point(312.75f,547.70f,244.65f),Point(312.75f,547.76f,314.75f),Point(243.25f,547.80f,314.55f),n);
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
    //scene.lights.push_back(al09); scene.numLights++;
    //scene.lights.push_back(al10); scene.numLights++;

    Point worldCenter;
    float worldRadius;
    ritterBoundingSphere(scene.vertices, worldCenter, worldRadius);
    std::cout << "worldCenter: " << worldCenter.X <<" "<< worldCenter.Y << " "<< worldCenter.Z << std::endl;
    std::cout << "worldRadius " << worldRadius << std::endl;

    HDRImageBuffer * imgEXR = load_exr("C:\\Users\\uncha\\Documents\\Masters\\VI\\Project\\VI-RT\\grace.exr");
    InfiniteAreaLight * light = new InfiniteAreaLight(imgEXR, worldCenter, worldRadius);
    scene.lights.push_back(light);
    scene.printSummary();


    //throw std::invalid_argument("AddPositiveIntegers arguments must be positive");

  
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
    OurRGB background(0,0,0);
    shd = new PathTracerShader(&scene, background);
    // declare the renderer
    StandardRenderer myRender(cam, &scene, img, shd);

    // start execution time
    auto start = high_resolution_clock::now();

    // render
    myRender.Render(SPP);
    
    // myRender.RenderParallelOpenMP(NUM_THREADS,SPP);
    //myRender.RenderParallel(NUM_THREADS,SPP);

    // stop execution time
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<seconds>(stop - start);


    cout << "Execution time: " << duration.count() << " seconds" << endl;

    // save the image
    img->Save("32spp_com_rr.ppm");
    
    cout << "That's all, folks!" << endl;
    return 0;
}
