//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"

void StandardRenderer::Render(int spp) {
    int W=0,H=0;  // resolution
    int x,y;

    // get resolution from the camera
    this->cam->getResolution(&W, &H);
    
    // main rendering loop: get primary rays from the camera until done
    for (y=0; y<H; y++) { 
        
        for (x=0; x<W; x++) { // loop over columns
            OurRGB color(0.,0.,0.);

            for(int ss=0; ss<spp; ss++) {
                Ray primary; Intersection isect; bool intersected; OurRGB this_color;
            
                // I) AMOSTRAGEM DE MONTE CARLO DO PIXEL
                float jitterV[2];
                jitterV[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                jitterV[1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            
                // Generate Ray (camera)
                cam->GenerateRay(x,y,&primary,jitterV);
                // trace ray (scene)
                intersected = scene->trace(primary, &isect);
                // shade this intersection (shader)
                this_color = shd->shade(intersected, isect, 0, primary);
                color += this_color;
            }

            color = color / (float) spp;

            // write the result into the image frame buffer (image)
            img->set(x,y,color);

            
        }
    }
}


void RenderThread(StandardRenderer* renderer, int threadId, int numThreads, int spp) {
    int W, H;
    renderer->cam->getResolution(&W, &H);

    srand(threadId);

    int cubeSize = 16; // Size of each cube
    int numCubesPerRow = W / cubeSize;
    int numCubesPerCol = H / cubeSize;
    int totalCubes = numCubesPerRow * numCubesPerCol;

    for (int cubeIdx = threadId; cubeIdx < totalCubes; cubeIdx += numThreads) {
        int cubeRow = cubeIdx / numCubesPerRow;
        int cubeCol = cubeIdx % numCubesPerRow;

        int startY = cubeRow * cubeSize;
        int startX = cubeCol * cubeSize;

        for (int y = startY; y < startY + cubeSize; y++) {
            for (int x = startX; x < startX + cubeSize; x++) {
                OurRGB color(0.,0.,0.);
            
                for(int ss=0; ss<spp; ss++) {
                    Ray primary; Intersection isect; bool intersected; OurRGB this_color;
                    
                    // I) AMOSTRAGEM DE MONTE CARLO DO PIXEL
                    float jitterV[2];
                    jitterV[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                    jitterV[1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                
                    // Generate Ray (camera)
                    renderer->cam->GenerateRay(x,y,&primary,jitterV);
                    
                    // trace ray (scene)
                    intersected = renderer->scene->trace(primary, &isect);

                    // shade this intersection (shader)
                    this_color = renderer->shd->shade(intersected, isect, 0, primary);
                    color += this_color;
                }

                color = color / (float) spp;

                // write the result into the image frame buffer (image)
                renderer->img->set(x,y,color);
            }
        }
    }
}


void StandardRenderer::RenderParallel(int numThreads, int spp) {
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; i++)
        threads.emplace_back(RenderThread, this, i, numThreads, spp);

    for (auto& thread : threads)
        thread.join();
}