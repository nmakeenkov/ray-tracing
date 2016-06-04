#ifndef GRAPHIC_LIBRARY_GEOMETRY3D_TRIANGLE_H
#define GRAPHIC_LIBRARY_GEOMETRY3D_TRIANGLE_H

#include "Shape.h"
#include "Plane.h"

namespace Geometry3d {

    class Triangle : Shape {
    public:
        Triangle();
        Triangle(Vector const &, Vector const &, Vector const &);

        Vector const &getPoint(int index) const;

        virtual Intersection intersect(Ray const &) const;
        virtual std::vector<Vector> getPoints() const;
        virtual Vector getNormal(Vector const &point) const;
        virtual Shape *clone() const;
    private:
        Vector mPoints[3];
        Vector mNormal;
        Plane mPlane;
    };

}

#endif // GRAPHIC_LIBRARY_GEOMETRY3D_TRIANGLE_H
