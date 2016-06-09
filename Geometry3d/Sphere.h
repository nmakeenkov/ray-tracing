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
        enum Projection { MERCATOR, EQUIRECTANGULAR };

        Sphere();
        Sphere(Vector const &center, double radius, Projection projection = EQUIRECTANGULAR);

        virtual Intersection intersect(Ray const &ray) const;
        virtual std::vector<Vector> getPoints() const;
        virtual Vector getNormal(Vector const &point) const;
        virtual Shape* clone() const;

        virtual std::pair<double, double> getPoint2d(Vector const &point3d) const;
    private:
        Vector mCenter;
        double mRadius;
        Projection mProjection;
    };

}

#endif // GRAPHIC_LIBRARY_GEOMETRY3D_SPHERE_H
