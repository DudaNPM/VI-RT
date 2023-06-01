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
                if (y == 374 && x == 332 && ss == 24){
                    std::cout << "Inicial" <<std::endl;
                }
            

                
                // I) AMOSTRAGEM DE MONTE CARLO DO PIXEL
                float jitterV[2];
                jitterV[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                jitterV[1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            
                // Generate Ray (camera)
                cam->GenerateRay(x,y,&primary,jitterV);
                    if (y == 374 && x == 332 && ss == 24){
                    std::cout << "Inicial2" <<std::endl;
                }
                // trace ray (scene)
                intersected = scene->trace(primary, &isect);
                if (y == 374 && x == 332 && ss == 24){
                    std::cout << "Inicial3" <<std::endl;
                }
                
                // shade this intersection (shader)
                this_color = shd->shade(intersected, isect, 0, primary);
                if (y == 374 && x == 332 && ss == 24){
                    std::cout << "Inicial4" <<std::endl;
                }
                color += this_color;

            }

            color = color / (float) spp;
            
            // write the result into the image frame buffer (image)
            img->set(x,y,color);

            
        }

    }
    std::cout<<"YOOOOO"<<std::endl;
}


/*
void RenderThread(StandardRenderer* renderer, int threadId, int numThreads, int spp) {
    int W, H;
    renderer->cam->getResolution(&W, &H);

    int start = (H / numThreads) * threadId;
    int final = (H / numThreads) * (threadId + 1);
    std::cout<<"YOOOOO"<<std::endl;

    srand(threadId);

    for (int y = start; y < final && y < H; y++) {
        for (int x = 0; x < W; x++) {
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
                this_color = renderer->shd->shade(intersected, isect, 0,primary);
                color += this_color;
            }

            color = color / (float) spp;
            // write the result into the image frame buffer (image)
            renderer->img->set(x,y,color);
        }
    }
}
*/




void RenderThread(StandardRenderer* renderer, int threadId, int numThreads, int spp) {
    int W, H;
    renderer->cam->getResolution(&W, &H);

    srand(threadId);

    for (int y = threadId; y < H; y += numThreads) {
        for (int x = 0; x < W; x++) {
            RGB color(0.,0.,0.);
            
            for(int ss=0; ss<spp; ss++) {
                Ray primary; Intersection isect; bool intersected; RGB this_color;
                
                // I) AMOSTRAGEM DE MONTE CARLO DO PIXEL
                float jitterV[2];
                jitterV[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                jitterV[1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            
                // Generate Ray (camera)
                renderer->cam->GenerateRay(x,y,&primary,jitterV);
                
                // trace ray (scene)
                intersected = renderer->scene->trace(primary, &isect);

                // shade this intersection (shader)
                this_color = renderer->shd->shade(intersected, isect, 0);
                color += this_color;
            }

            color = color / (float) spp;
            // write the result into the image frame buffer (image)
            renderer->img->set(x,y,color);
        }
    }
}




void StandardRenderer::RenderParallel(int numThreads, int spp) {
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back(RenderThread, this, i, numThreads, spp);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}



void StandardRenderer::RenderParallelOpenMP(int num_threads, int spp) {
    int W=0,H=0;  // resolution

    // get resolution from the camera
    this->cam->getResolution(&W, &H);
    
    // main rendering loop: get primary rays from the camera until done
    #pragma omp parallel for num_threads(num_threads)
    for (int y=0; y<H; y++) {  // loop over rows
        for (int x=0; x<W; x++) { // loop over columns
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
                this_color = shd->shade(intersected, isect, 0,primary);
                color += this_color;
            }

            color = color / (float) spp;
            // write the result into the image frame buffer (image)
            img->set(x,y,color);
        }
    }
}