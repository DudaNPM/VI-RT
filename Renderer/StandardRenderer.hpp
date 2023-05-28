//
//  StandardRenderer.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//


#ifndef StandardRenderer_hpp
#define StandardRenderer_hpp

#include "renderer.hpp"


class StandardRenderer: public Renderer {
public:
    StandardRenderer (Camera *cam, Scene * scene, Image * img, Shader *shd): Renderer(cam, scene, img, shd) {}
    void Render(int spp);
    void RenderParallelOpenMP(int num_threads, int spp);
    void RenderParallel(int numThreads, int spp);
};

#endif /* StandardRenderer_hpp */
