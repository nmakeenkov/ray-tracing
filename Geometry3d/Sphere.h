#ifndef GRAPHIC_LIBRARY_GEOMETRY3D_SPHERE_H
#define GRAPHIC_LIBRARY_GEOMETRY3D_SPHERE_H

#include "Shape.h"
#include "Vector.h"

namespace Geometry3d {

    class Sphere : Shape {
    public:
        Sphere();
        Sphere(Vector const &center, double radius);

    private:
        Vector mCenter;
        double mRadius;
    };

}

#endif // GRAPHIC_LIBRARY_GEOMETRY3D_SPHERE_H
