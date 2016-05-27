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

        virtual Intersection intersect(Ray const &ray) const {
            // |ray.start + t * ray.direction - center| = r
            // |ray.start + t * ray.direction - center| ^ 2 = r ^ 2
            auto p0 = ray.mStart - mCenter;
            // a * t^2 + b * t + c = 0
            double a = Utils::sqr(ray.mDirection.mX) + Utils::sqr(ray.mDirection.mY) + Utils::sqr(ray.mDirection.mZ);
            double b = 2. * (p0.mX * ray.mDirection.mX + p0.mY * ray.mDirection.mY + p0.mZ * ray.mDirection.mZ);
            double c = Utils::sqr(p0.mX) + Utils::sqr(p0.mY) + Utils::sqr(p0.mZ) - Utils::sqr(mRadius);
            double D = Utils::sqr(b) - 4. * a * c;
            if (D < EPS) {
                return Intersection();
            }
            D = sqrt(D);
            std::vector<Vector> result(2);
            double t0 = (-b - D) / (2. * a);
            double t1 = (-b + D) / (2. * a);
            if (t0 > t1) {
                std::swap(t0, t1);
            }
            if (t0 > EPS) {
                std::vector<Vector> result(2);
                result[0] = ray.mStart + t0 * ray.mDirection;
                result[1] = ray.mStart + t1 * ray.mDirection;
                return Intersection(result);
            }
            if (t1 > EPS) {
                return Intersection(std::vector<Vector>(1, ray.mStart + t1 * ray.mDirection));
            }
            return Intersection();
        }
        virtual std::vector<Vector> getPoints() const {
            std::vector<Vector> result;
            // for bounding box
            result.push_back(mCenter + Vector(mRadius, mRadius, mRadius));
            result.push_back(mCenter - Vector(mRadius, mRadius, mRadius));
            return result;
        }
        virtual Vector getNormal(Vector const &point) const {
            auto ans = point - mCenter;
            return ans / ans.abs();
        }
        virtual Shape* clone() const {
            return new Sphere(*this);
        }
    private:
        Vector mCenter;
        double mRadius;
    };

}

#endif // GRAPHIC_LIBRARY_GEOMETRY3D_SPHERE_H
