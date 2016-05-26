#ifndef GRAPHIC_LIBRARY_GEOMETRY3D_PLANE_H
#define GRAPHIC_LIBRARY_GEOMETRY3D_PLANE_H

#include "Vector.h"
#include "Intersection.h"

namespace Geometry3d {

    class Plane {
    public:
        Plane();
        Plane(double a, double b, double c, double d); // ax + by + cz + d = 0
        Plane(Vector const &, Vector const &, Vector const &);

        Intersection intersect(Ray const &) const;
    private:
        double mA, mB, mC, mD;
    };

};

#endif // GRAPHIC_LIBRARY_GEOMETRY3D_PLANE_H