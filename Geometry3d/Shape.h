#ifndef GRAPHIC_LIBRARY_GEOMETRY3D_SHAPE_H
#define GRAPHIC_LIBRARY_GEOMETRY3D_SHAPE_H

#include "Intersection.h"
#include "Vector.h"

namespace Geometry3d {

    class Shape {
    public:
        Shape() {}
//        virtual Intersection intersect(Segment const &) const = 0;
//        virtual Intersection intersect(Line const &) const = 0;
        virtual Intersection intersect(Ray const &) const = 0;
//        virtual Intersection intersect(Sphere const &) const = 0;
//        virtual Intersection intersect(Triangle const &) const = 0;
//        virtual Intersection intersect(Quadrangle const &) const = 0;
//        virtual Intersection intersect(Parallelogram const &) const = 0;
        virtual std::vector<Vector> getPoints() const = 0;
        virtual Vector getNormal(Vector const &point) const = 0;
        virtual std::pair<double, double> getPoint2d(Vector const &point3d) const = 0;
        virtual Shape* clone() const = 0;
    };

}

#endif
