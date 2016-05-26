#include "Plane.h"
#include "Ray.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace Geometry3d;

Plane::Plane() { }

Plane::Plane(double a, double b, double c, double d) : mA(a), mB(b), mC(c), mD(d) { }

Plane::Plane(const Vector &f, const Vector &s, const Vector &t) {
     // need: ax + by + cz + d = 0

    if (fabs(f.mX - s.mX) < EPS && fabs(f.mX - t.mX) < EPS) {
        mA = 1;
        mB = 0;
        mC = 0;
        mD = -f.mX;
        return;
    }

    // linear: ax + by + cz = 0
    Vector linear1 = f - s;
    Vector linear2 = f - t;

    // cool: by + cz = 0
    Vector cool;
    if (fabs(linear1.mX) < EPS) {
        cool = linear1;
    } else {
        if (fabs(linear2.mX) < EPS) {
            cool = linear2;
        } else {
            cool = linear2.mX * linear1 - linear1.mX * linear2;
        }
    }

    // can find b and c
    mB = cool.mZ;
    mC = -cool.mY;

    // find a from linear, in at least one of them x != 0 as we would then exit in the beginning
    if (fabs(linear1.mX) < EPS) {
        mA = (-mB * linear2.mY - mC * linear2.mZ) / linear2.mX;
    } else {
        mA = (-mB * linear1.mY - mC * linear1.mZ) / linear1.mX;
    }

    // finally find d
    mD = -mA * f.mX - mB * f.mY - mC * f.mZ;

//    cerr << mA * f.mX + mB * f.mY + mC * f.mZ + mD << "\n" <<
//            mA * s.mX + mB * s.mY + mC * s.mZ + mD << "\n" <<
//            mA * t.mX + mB * t.mY + mC * t.mZ + mD << "\n" << endl;
}

Intersection Plane::intersect(const Ray &ray) const {
    double startSummand = ray.mStart.mX * mA + ray.mStart.mY * mB + ray.mStart.mZ * mC + mD;
    double directionSummand = ray.mDirection.mX * mA + ray.mDirection.mY * mB + ray.mDirection.mZ * mC;
    if (fabs(directionSummand) < EPS) {
        if (fabs(startSummand) < EPS) {
            return Intersection(new Ray(ray));
        } else {
            return Intersection(); // NONE, as it's parallel
        }
    }
    double t = -startSummand / directionSummand;
    if (t < 0) {
        return  Intersection(); // NONE, on the line, but not ray
    }

    Vector f(ray.mStart + t * ray.mDirection);
    double dist = mA * f.mX + mB * f.mY + mC * f.mZ + mD;
    if (fabs(dist) > EPS) {
        cerr << dist << endl;
    }

    return Intersection(vector<Vector>(1, Vector(ray.mStart + t * ray.mDirection))); // the point
}