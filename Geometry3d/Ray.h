#ifndef GRAPHIC_LIBRARY_GEOMETRY3D_RAY_H
#define GRAPHIC_LIBRARY_GEOMETRY3D_RAY_H

#include "Vector.h"

namespace Geometry3d {

    class Plane;

    class Ray {
    public:
        Ray();
        Ray(Vector const &start, Vector const &point);
        friend class Plane;

        Vector mStart, mDirection;
    };

}

#endif // GRAPHIC_LIBRARY_GEOMETRY3D_RAY_H
