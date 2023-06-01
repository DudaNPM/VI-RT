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


// Normalization
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


void ImagePPM::ToneMap3() {
    imageToSave = new PPM_pixel[W*H];
    
    // loop over each pixel in the image, clamp and convert to byte format
    for (int j = 0 ; j< H ; j++) {
        for (int i = 0; i < W ; ++i) {
            imageToSave[j*W+i].val[0] = (unsigned char)((imagePlane[j*W+i].R / (imagePlane[j*W+i].R + 1)) * 255);
            imageToSave[j*W+i].val[1] = (unsigned char)((imagePlane[j*W+i].G / (imagePlane[j*W+i].G + 1)) * 255);
            imageToSave[j*W+i].val[2] = (unsigned char)((imagePlane[j*W+i].B / (imagePlane[j*W+i].B + 1)) * 255);
        }
    }
}


// Gamma compreenssion
void ImagePPM::ToneMap4() {
    imageToSave = new PPM_pixel[W*H];
    float a = 1.0f; // change brightness
    float gamma = 1.2f; // change contrast
    
    // loop over each pixel in the image, clamp and convert to byte format
    for (int j = 0 ; j< H ; j++) {
        for (int i = 0; i < W ; ++i) {
            imageToSave[j*W+i].val[0] = (unsigned char)((a * powf(imagePlane[j*W+i].R, gamma)) * 255);
            imageToSave[j*W+i].val[1] = (unsigned char)((a * powf(imagePlane[j*W+i].G, gamma)) * 255);
            imageToSave[j*W+i].val[2] = (unsigned char)((a * powf(imagePlane[j*W+i].B, gamma)) * 255);
        }
    }
}


// Drago et al: ((L_in * (1 + (L_in / L_white^2))) / (1 + L_in))
void ImagePPM::ToneMap5() {
    imageToSave = new PPM_pixel[W*H];
    
    RGB max = RGB();

    for (int j = 0 ; j< H ; j++) {
        for (int i = 0; i < W ; ++i) {
            if (imagePlane[j*W+i].R > max.R) max.R = imagePlane[j*W+i].R;
            if (imagePlane[j*W+i].G > max.G) max.G = imagePlane[j*W+i].G;
            if (imagePlane[j*W+i].B > max.B) max.B = imagePlane[j*W+i].B;
        }
    }
    
    // loop over each pixel in the image, clamp and convert to byte format
    for (int j = 0 ; j< H ; j++) {
        for (int i = 0; i < W ; ++i) {
            imageToSave[j*W+i].val[0] = (unsigned char)(((imagePlane[j*W+i].R * (1 + (imagePlane[j*W+i].R / powf(0.4f,2)))) / (1 + imagePlane[j*W+i].R)) * 255);
            imageToSave[j*W+i].val[1] = (unsigned char)(((imagePlane[j*W+i].G * (1 + (imagePlane[j*W+i].G / powf(0.4f,2)))) / (1 + imagePlane[j*W+i].G)) * 255);
            imageToSave[j*W+i].val[2] = (unsigned char)(((imagePlane[j*W+i].B * (1 + (imagePlane[j*W+i].B / powf(0.4f,2)))) / (1 + imagePlane[j*W+i].B)) * 255);
        }
    }
}


// Reinhard et al. tone mapping
void ImagePPM::ToneMap6() {
    imageToSave = new PPM_pixel[W * H];
    
    // Tone mapping parameters
    float key = 1.5f;  // Key value for Reinhard et al. tone mapping
    float L_white = 1.0f;  // Maximum luminance value in the scene
    
    // Loop over each pixel in the image, apply tone mapping, clamp, and convert to byte format
    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; ++i) {
            // Retrieve the pixel color from the HDR image
            RGB pixelColor = imagePlane[j * W + i];
            
            // Apply Reinhard et al. tone mapping
            RGB tonedPixelColor = pixelColor / (pixelColor + key);
            tonedPixelColor = tonedPixelColor * ((tonedPixelColor / (L_white * L_white)) + 1.0f);
            
            // Clamp the pixel color to ensure it stays within the valid range
            tonedPixelColor.Clamp();
            
            // Convert the toned pixel color to byte format and store it in the LDR image
            imageToSave[j * W + i].val[0] = (unsigned char)(tonedPixelColor.R * 255);
            imageToSave[j * W + i].val[1] = (unsigned char)(tonedPixelColor.G * 255);
            imageToSave[j * W + i].val[2] = (unsigned char)(tonedPixelColor.B * 255);
        }
    }
}


// ACES tone mapping
void ImagePPM::ToneMap7() {
    imageToSave = new PPM_pixel[W * H];

    // ACES tone mapping parameters
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    
    // Loop over each pixel in the image, apply tone mapping, clamp, and convert to byte format
    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; ++i) {
            // Retrieve the pixel color from the HDR image
            RGB pixelColor = imagePlane[j * W + i];
            
            // Apply ACES tone mapping
            RGB mappedColor;
            mappedColor.R = (pixelColor.R * (a * pixelColor.R + b)) / (pixelColor.R * (c * pixelColor.R + d) + e);
            mappedColor.G = (pixelColor.G * (a * pixelColor.G + b)) / (pixelColor.G * (c * pixelColor.G + d) + e);
            mappedColor.B = (pixelColor.B * (a * pixelColor.B + b)) / (pixelColor.B * (c * pixelColor.B + d) + e);
            
            // Clamp the pixel color to ensure it stays within the valid range
            mappedColor.Clamp();
            
            // Convert the toned pixel color to byte format and store it in the LDR image
            imageToSave[j * W + i].val[0] = (unsigned char)(mappedColor.R * 255);
            imageToSave[j * W + i].val[1] = (unsigned char)(mappedColor.G * 255);
            imageToSave[j * W + i].val[2] = (unsigned char)(mappedColor.B * 255);
        }
    }
}


RGB Mantiuk_ToneMap(RGB& hdrColor, float contrastFactor, float saturationFactor, float detailFactor) {
    float luminance = hdrColor.Y();     // Represents the lightness or luminance of the color
    float a = hdrColor.R - hdrColor.G;  // Represents the color component along the red-green axis
    float b = hdrColor.B - hdrColor.G;  // Represents the color component along the yellow-blue axis

    // Apply Mantiuk et al. tone mapping operator
    luminance *= contrastFactor;
    luminance = std::tanhf(luminance);

    a *= saturationFactor;
    b *= saturationFactor;

    luminance *= detailFactor;

    // Convert the perceptual color back to HDR color space
    RGB mappedColor;
    mappedColor.R = luminance + a;
    mappedColor.G = luminance;
    mappedColor.B = luminance + b;

    return mappedColor;
}


// Mantiuk tone mapping
void ImagePPM::ToneMap8() {
    imageToSave = new PPM_pixel[W * H];

    // Tone mapping parameters
    float contrastFactor = 0.4f;    // Contrast factor
    float saturationFactor = 0.4f;  // Saturation factor
    float detailFactor = 1.0f;      // Detail factor
    
    // Loop over each pixel in the image, apply tone mapping, clamp, and convert to byte format
    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; ++i) {
            // Retrieve the pixel color from the HDR image
            RGB pixelColor = imagePlane[j * W + i];
            
            // Apply Mantiuk tone mapping
            RGB mappedColor = Mantiuk_ToneMap(pixelColor, contrastFactor, saturationFactor, detailFactor);
            
            // Clamp the pixel color to ensure it stays within the valid range
            mappedColor.Clamp();
            
            // Convert the toned pixel color to byte format and store it in the LDR image
            imageToSave[j * W + i].val[0] = (unsigned char)(mappedColor.R * 255);
            imageToSave[j * W + i].val[1] = (unsigned char)(mappedColor.G * 255);
            imageToSave[j * W + i].val[2] = (unsigned char)(mappedColor.B * 255);
        }
    }
}



bool ImagePPM::Save (std::string filename) {
    // convert from float to {0,1,..., 255}
    // ToneMap();
    // ToneMap2();
    // ToneMap3();
    // ToneMap4();
    // ToneMap5();
    // ToneMap6();
    // ToneMap7();
    ToneMap8();

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
