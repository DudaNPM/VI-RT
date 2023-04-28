#ifndef triangle_hpp
#define triangle_hpp

#include "geometry.hpp"

class Triangle: public Geometry {
public:
    Point v1, v2, v3;
    Vector normal, edge1, edge2;
    BB bb;
    
    /*
    bool intersect (Ray r, Intersection *isect) {
        // intersect the ray with the triangle boundind box
        if (!this->bb.intersect(r)) return false;

        const float EPSILON = 0.0000001f;
        Vector h, s, q;
        float a,f,u,v;

        h = r.dir.cross(edge2);
        a = edge1.dot(h);
    
        if (a > -EPSILON && a < EPSILON) // a == 0
            return false;    // This ray is parallel to this triangle.

        f = (float) 1.0 / a;
        s = v1.vec2point(r.o);
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
            Vector v = r.dir * t;
            Point intPoint = r.o.operator+(Point(v.X,v.Y,v.Z));
            Vector wo = -1.f * r.dir;
            this->normal.Faceforward(wo); // make sure the normal points to the same side of the surface as wo
            
            // Guardar info do ponto de interseção
            isect->p = intPoint;
            isect->gn = isect->sn = this->normal;
            isect->wo = wo;
            isect->depth = t;
            
            return true;
        }
        else // This means that there is a line intersection but not a ray intersection.
            return false;
    }
    */
    
    Triangle(Point _v1, Point _v2, Point _v3, Vector _normal): v1(_v1), v2(_v2), v3(_v3), normal(_normal) {
        edge1 = v1.vec2point(v2);
        edge2 = v1.vec2point(v3);
        bb.min.set(v1.X, v1.Y, v1.Z);
        bb.max.set(v1.X, v1.Y, v1.Z);
        bb.update(v2);
        bb.update(v3);
    }

    // https://www.mathopenref.com/heronsformula.html
    float area () {
        float a = v1.distance(v2);
        float b = v1.distance(v3);
        float c = v2.distance(v3);
        float p = (a + b + c) / 2.0f;
        float area = sqrt(p * (p-a) * (p-b) * (p-c));
        return area;
    }
};

#endif /* triangle_hpp */