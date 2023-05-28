//
//  ImagePPM.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//


#include "ImagePPM.hpp"


void ImagePPM::ToneMap() {
    imageToSave = new PPM_pixel[W*H];
    
    // loop over each pixel in the image, clamp and convert to byte format
    for (int j = 0 ; j< H ; j++) {
        for (int i = 0; i < W ; ++i) {
            imageToSave[j*W+i].val[0] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].R) * 255);
            imageToSave[j*W+i].val[1] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].G) * 255);
            imageToSave[j*W+i].val[2] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].B) * 255);
        }
    }
}


void ImagePPM::ToneMap2() {
    imageToSave = new PPM_pixel[W*H];

    RGB max = RGB();
    RGB min = RGB(RAND_MAX,RAND_MAX,RAND_MAX);

    for (int j = 0 ; j< H ; j++) {
        for (int i = 0; i < W ; ++i) {
            if (imagePlane[j*W+i].R > max.R) max.R = imagePlane[j*W+i].R;
            else if (imagePlane[j*W+i].R < min.R) min.R = imagePlane[j*W+i].R;
            if (imagePlane[j*W+i].G > max.G) max.G = imagePlane[j*W+i].G;
            else if (imagePlane[j*W+i].G < min.G) min.G = imagePlane[j*W+i].G;
            if (imagePlane[j*W+i].B > max.B) max.B = imagePlane[j*W+i].B;
            else if (imagePlane[j*W+i].B < min.B) min.B = imagePlane[j*W+i].B;
        }
    }
    
    // loop over each pixel in the image, clamp and convert to byte format
    for (int j = 0 ; j< H ; j++) {
        for (int i = 0; i < W ; ++i) {
            imageToSave[j*W+i].val[0] = (unsigned char) (std::min((imagePlane[j*W+i].R - min.R / (max.R - min.R)), 1.f) * 255.f);
            imageToSave[j*W+i].val[1] = (unsigned char) (std::min((imagePlane[j*W+i].G - min.G / (max.G - min.G)), 1.f) * 255.f);
            imageToSave[j*W+i].val[2] = (unsigned char) (std::min((imagePlane[j*W+i].B - min.B / (max.B - min.B)), 1.f) * 255.f);
        }
    }
}


bool ImagePPM::Save (std::string filename) {
    // convert from float to {0,1,..., 255}
    ToneMap();
    // ToneMap2();

    // write imageToSave to file
    std::ofstream ofs;
    
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail()) throw("Can't open output file");
        ofs << "P6\n" << W << " " << H << "\n255\n";
        unsigned char r, g, b;
        
        for (int j = 0 ; j< H ; j++) {
            for (int i = 0; i < W ; ++i) {
                r = imageToSave[j*W+i].val[0];
                g = imageToSave[j*W+i].val[1];
                b = imageToSave[j*W+i].val[2];
                ofs << r << g << b;
            }
        }
        
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }

    // Details and code on PPM files available at:
    // https://www.scratchapixel.com/lessons/digital-imaging/simple-image-manipulations/reading-writing-images.html
    
    return true;
}
