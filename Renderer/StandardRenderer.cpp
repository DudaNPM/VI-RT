//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"
#include "Phong.hpp"

void StandardRenderer::Render () {
    int W=0,H=0;  // resolution
    int x,y;

    // get resolution from the camera
    this->cam->getResolution(&W, &H);
    
    // main rendering loop: get primary rays from the camera until done
    for (y=0 ; y< H ; y++) {  // loop over rows
        for (x=0 ; x< W ; x++) { // loop over columns
            Ray primary;
            Intersection isect;
            bool intersected;
            RGB color;
          
            // Generate Ray (camera)
            this->cam->GenerateRay(x,y,&primary);

            if (x==W/2 && y == H/2) {
                std::cout << "este e o dir" << std::endl;
                std::cout << primary.dir.X << std::endl;
                std::cout << primary.dir.Y << std::endl;
                std::cout << primary.dir.Z << std::endl;
            }
            
            // trace ray (scene)
            intersected = this->scene->trace(primary, &isect);

            // shade this intersection (shader)
            color = this->shd->shade(intersected, isect);

            // write the result into the image frame buffer (image)
            img->set(x,y,color);
            
        } // loop over columns
    }   // loop over rows
}
