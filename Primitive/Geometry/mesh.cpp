//
//  mesh.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#include "mesh.hpp"
#include <limits>


Mesh::Mesh(int numFaces, int numVertices, int numNormals){
    numFaces = numFaces;
    numVertices = numVertices;
    numNormals = numNormals;
}

void Mesh::updateBBMesh(std::vector<Point> vertices){
    for (auto i = 0; i < faces.size(); ++i){
        for (auto j = 0; j < 3; ++j){
            Point p = vertices.at(faces[i].vert_ndx[j]);
            this->bb.update(p);
        }
    }
}
// see pbrt book (3rd ed.), sec 3.6.2, pag 157
bool Mesh::TriangleIntersect (Ray r, Face f, Intersection *isect) {


    return false;
}

bool Mesh::intersect (Ray r, Intersection *isect) {
    bool intersect = true, intersect_this_face;
    Intersection min_isect, curr_isect;
    float min_depth=std::numeric_limits<float>::max();
    // intersect the ray with the mesh BB
    
    if (!intersect) return false;
    
    // If it intersects then loop through the faces
    intersect = false;
    for (auto face_it=faces.begin() ; face_it != faces.end() ; face_it++) {
        intersect_this_face = TriangleIntersect(r, *face_it, &curr_isect);
        if (!intersect_this_face) continue;
        
        intersect = true;
        if (curr_isect.depth < min_depth) {  // this is closer
            min_depth = curr_isect.depth;
            min_isect = curr_isect;
        }
    }
    
    return intersect;
}
