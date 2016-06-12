#ifndef GRAPHIC_LIBRARY_GEOMETRY3D_PARALLELOGRAM_H
#define GRAPHIC_LIBRARY_GEOMETRY3D_PARALLELOGRAM_H

#include "Shape.h"
#include "Plane.h"

namespace Geometry3d {

    class Parallelogram : Shape {
    public:
        Parallelogram();
        Parallelogram(Vector const &, Vector const &, Vector const &); // 3 points
        // in detour order

        Vector const &getPoint(int index) const;

        virtual Intersection intersect(Ray const &) const;
        virtual std::vector<Vector> getPoints() const;
        virtual Vector getNormal(Vector const &point) const;
        virtual std::pair<double, double> getPoint2d(Vector const &point3d) const;
        virtual Shape *clone() const;
    private:
        Vector mPoints[4];
        Vector mNormal;
        Plane mPlane;
        Vector mAxis0, mAxis1;
        double mAxis0Length, mAxis1Length;
    };

}

#endif // GRAPHIC_LIBRARY_GEOMETRY3D_PARALLELOGRAM_H
