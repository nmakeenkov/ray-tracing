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
        virtual Vector getNormal() const {
            auto ans = Vector::vectorProduction(mPoints[1] - mPoints[0], mPoints[2] - mPoints[0]);
            return ans / ans.abs();
        }
        virtual Shape *clone() const;
    private:
        Vector mPoints[3];
        Plane mPlane;
    };

}

#endif // GRAPHIC_LIBRARY_GEOMETRY3D_TRIANGLE_H
