#ifndef GRAPHIC_LIBRARY_GEOMETRY3D_INTERSECTION_H
#define GRAPHIC_LIBRARY_GEOMETRY3D_INTERSECTION_H

#include <vector>
#include "Vector.h"

namespace Geometry3d {

    class Ray;

    class Intersection {
    public:
        enum IntersectionResult {
            NONE, POINTS, RAY, SEGMENT
        };

        Intersection();
        Intersection(std::vector<Vector> const &);
        Intersection(Ray *);

        IntersectionResult mResult;
        std::vector<Vector> mPoints;
        Ray *mRay;
    };

}

#endif // GRAPHIC_LIBRARY_GEOMETRY3D_INTERSECTION_H
