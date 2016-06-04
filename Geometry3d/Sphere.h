#ifndef GRAPHIC_LIBRARY_GEOMETRY3D_SPHERE_H
#define GRAPHIC_LIBRARY_GEOMETRY3D_SPHERE_H

#include "Shape.h"
#include "Vector.h"
#include "Ray.h"
#include "Utils.h"
#include <cmath>
#include <iostream>

namespace Geometry3d {

    class Sphere : Shape {
    public:
        Sphere();
        Sphere(Vector const &center, double radius);

        virtual Intersection intersect(Ray const &ray) const;
        virtual std::vector<Vector> getPoints() const;
        virtual Vector getNormal(Vector const &point) const;
        virtual Shape* clone() const;
    private:
        Vector mCenter;
        double mRadius;
    };

}

#endif // GRAPHIC_LIBRARY_GEOMETRY3D_SPHERE_H
