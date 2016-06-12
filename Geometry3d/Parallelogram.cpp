#include "Parallelogram.h"

using namespace std;
using namespace Geometry3d;

Parallelogram::Parallelogram() : Shape() { }

Parallelogram::Parallelogram(const Vector &f, const Vector &s, const Vector &t) : mPlane(f, s, t), mAxis0(s - f), mAxis1(t - f) {
    mPoints[0] = f;
    mPoints[1] = s;
    mPoints[2] = t;
    mPoints[3] = t + f - s;
    mNormal = Vector::vectorProduction(mPoints[1] - mPoints[0], mPoints[2] - mPoints[0]);
    mNormal = mNormal / mNormal.abs();
    mAxis0Length = mAxis0.abs();
    mAxis0 = mAxis0 / mAxis0Length;
    mAxis1Length = mAxis1.abs();
    mAxis1 = mAxis1 / mAxis1Length;

    mAxis0Length = 100;
    mAxis1Length = 100;
}

Vector const& Parallelogram::getPoint(int index) const {
    return mPoints[index];
}

Intersection Parallelogram::intersect(const Ray &ray) const {
    Intersection result = mPlane.intersect(ray);
    if (result.mResult == Intersection::NONE) {
        return result;
    }
    if (result.mResult == Intersection::POINTS) {
        // check if the point belongs to parallelogram
        Vector v[4];
        for (int i = 0; i < 4; ++i) {
            v[i] = mPoints[i] - result.mPoints[0];
        }
        double area = Vector::vectorProduction(v[0], v[3]).abs();
        for (int i = 0; i < 3; ++i) {
            area += Vector::vectorProduction(v[i], v[i + 1]).abs();
        }
        area /= 2.0;
        if (area < Vector::vectorProduction(mPoints[1] - mPoints[0],
                                            mPoints[3] - mPoints[0]).abs() + EPS) {
            return result;
        } else {
            return Intersection();
        }
    }
    // RAY -> NONE
    return Intersection();
}

Vector Parallelogram::getNormal(Vector const &point) const {
    return mNormal;
}

vector<Vector> Parallelogram::getPoints() const {
    vector<Vector> result;
    for (int i = 0; i < 4; ++i) {
        result.push_back(mPoints[i]);
    }
    return result;
}

Shape* Parallelogram::clone() const {
    return new Parallelogram(*this);
}

#include <iostream>

std::pair<double, double> Parallelogram::getPoint2d(Vector const &point3d) const {
    auto point = point3d - mPoints[0];
    return make_pair(Vector::scalarProduction(point, mAxis0) / mAxis0Length,
                     Vector::scalarProduction(point, mAxis1) / mAxis1Length);
}
