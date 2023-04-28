//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"
#include "Phong.hpp"
#include <stdlib.h>

void StandardRenderer::Render () {
    int W=0,H=0;  // resolution
    int x,y;

    // get resolution from the camera
    this->cam->getResolution(&W, &H);
    
    // main rendering loop: get primary rays from the camera until done
    const int spp=16;
    for (y=0; y<H; y++) {  // loop over rows
        for (x=0; x<W; x++) { // loop over columns
            RGB color(0.,0.,0.);
            
            for(int ss=0; ss<spp; ss++) {
                Ray primary;
                Intersection isect;
                bool intersected;
                RGB this_color;

                float jitterV[2] = {(rand() % 100) / 100, (rand() % 100) / 100};
            
                // Generate Ray (camera)
                this->cam->GenerateRay(x,y,&primary,jitterV);
                
                // trace ray (scene)
                intersected = this->scene->trace(primary, &isect);

                // shade this intersection (shader)
                this_color = this->shd->shade(intersected, isect);
                color += this_color;
            }

            color = color / RGB(spp,spp,spp);
            // write the result into the image frame buffer (image)
            img->set(x,y,color);
        }
    }
}
