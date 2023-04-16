//
//  mesh.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#include "mesh.hpp"
#include <limits>


Mesh::Mesh(int numFaces, int numVertices, int numNormals){
    this->numFaces = numFaces;
    this->numVertices = numVertices;
    this->numNormals = numNormals;
}

void Mesh::updateBBMesh(std::vector<Point> vertices){
    for (auto i = 0; i < faces.size(); ++i){
        for (auto j = 0; j < 3; ++j){
            Point p = vertices.at(faces[i].vert_ndx[j]);
            faces[i].bb.update(p);
            this->bb.update(p);
        }
    }
}

// see pbrt book (3rd ed.), sec 3.6.2, pag 157
// Algorithm explained: https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
bool Mesh::TriangleIntersect(std::vector<Point> vertices, Ray r, Face face, Intersection *isect) {
    // intersect the ray with the triangle boundind box
    if (!face.bb.intersect(r)) return false;

    const float EPSILON = 0.0000001f;
    Vector edge1, edge2, h, s, q;
    float a,f,u,v;

    // vertices da face/triangulo
    Point vertex0 = vertices.at(face.vert_ndx[0]);
    Point vertex1 = vertices.at(face.vert_ndx[1]);
    Point vertex2 = vertices.at(face.vert_ndx[2]);
    
    edge1 = vertex0.vec2point(vertex1);
    edge2 = vertex0.vec2point(vertex2);

    h = r.dir.cross(edge2);
    a = edge1.dot(h);
    
    if (a > -EPSILON && a < EPSILON) // a == 0
        return false;    // This ray is parallel to this triangle.

    f = (float) 1.0 / a;
    s = vertex0.vec2point(r.o);
    u = f * s.dot(h);

    if (u < 0.0 || u > 1.0)
        return false;
    
    q = s.cross(edge1);
    v = f * r.dir.dot(q);

    if (v < 0.0 || u + v > 1.0)
        return false;
    
    // At this stage we can compute t to find out where the intersection point is on the line.
    // t é o valor que nos faz "andar" no vetor direção do raio, varia entre 0 e inf
    // se for negativo significa que o raio esta a ir na direção contrária á do face/triangulo
    float t = f * edge2.dot(q);
    
    if (t > EPSILON) { // ray intersection
        // Ponto de interseção :: I = ray.o + (ray.dir * t)
        Vector v = r.dir.operator*(t); 
        Point intPoint = r.o.operator+(Point(v.X,v.Y,v.Z));
        Vector normal = face.geoNormal;
        Vector wo = -1.f * r.dir;
        normal.Faceforward(wo); // make sure the normal points to the same side of the surface as wo
        
        // Guardar info do ponto de interseção
        isect->p = intPoint;
        isect->gn = isect->sn = normal;
        isect->wo = wo;
        isect->depth = t;
        
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

bool Mesh::intersect(std::vector<Point> vertices, Ray r, Intersection *isect) {
    bool intersect = true, intersect_this_face;
    Intersection curr_isect;
    float min_depth = std::numeric_limits<float>::max();
    
    // intersect the ray with the mesh BB
    intersect = this->bb.intersect(r);
    if (!intersect) return false;
    
    // If it intersects then loop through the faces
    intersect = false;
    for (auto face_it = faces.begin(); face_it != faces.end(); face_it++) {
        
        intersect_this_face = TriangleIntersect(vertices, r, *face_it, &curr_isect);
        
        if (intersect_this_face) {
            intersect = true;
            if (curr_isect.depth < min_depth) {  // this is closer
                min_depth = curr_isect.depth;
                *isect = curr_isect;
            }
        }
    }
    
    return intersect;
}
