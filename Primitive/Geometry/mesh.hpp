//
//  mesh.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//


#ifndef mesh_hpp
#define mesh_hpp

#include "geometry.hpp"


// partially inspired in pbrt book (3rd ed.), sec 3.6, pag 152

typedef struct Face {
    std::vector<int> vert_ndx;  // indices to our internal vector of vertices (in Mesh)
    Vector geoNormal;           // geometric normal
    bool hasShadingNormals;     // are there per vertex shading normals ??
    std::vector<int> vert_normals_ndx;   // indices to veritices normals
    BB bb;      // face bounding box
                // this is min={0.,0.,0.} , max={0.,0.,0.} due to the Point constructor
} Face;

class Mesh: public Geometry {
private:
    bool TriangleIntersect(std::vector<Point> vertices, Ray r, Face face, Intersection *isect);
public:
    int numFaces;
    int numVertices;
    int numNormals;
    std::vector<Face> faces;
    bool intersect (std::vector<Point> vertices, Ray r, Intersection *isect);
    void updateBBMesh(std::vector<Point> vertices);
    
    Mesh(int numFaces, int numVertices, int numNormals);   
    Mesh(): numFaces(0), numVertices(0), numNormals(0) {}
};

#endif /* mesh_hpp */
