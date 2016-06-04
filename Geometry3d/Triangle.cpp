#include <iostream>
#include "Triangle.h"

using namespace std;
using namespace Geometry3d;

Triangle::Triangle() : Shape() { }

Triangle::Triangle(const Vector &f, const Vector &s, const Vector &t) : mPlane(f, s, t) {
    mPoints[0] = f;
    mPoints[1] = s;
    mPoints[2] = t;
    mNormal = Vector::vectorProduction(mPoints[1] - mPoints[0], mPoints[2] - mPoints[0]);
    mNormal = mNormal / mNormal.abs();
}

Vector const& Triangle::getPoint(int index) const {
    return mPoints[index];
}

Intersection Triangle::intersect(const Ray &ray) const {
    Intersection result = mPlane.intersect(ray);
    if (result.mResult == Intersection::NONE) {
        return result;
    }
    if (result.mResult == Intersection::POINTS) {
        // check if the point belongs to triangle
        Vector v[3];
        for (int i = 0; i < 3; ++i) {
            v[i] = mPoints[i] - result.mPoints[0];
        }
        double area = Vector::vectorProduction(v[0], v[2]).abs();
        for (int i = 0; i < 2; ++i) {
            area += Vector::vectorProduction(v[i], v[i + 1]).abs();
        }
        if (area < Vector::vectorProduction(mPoints[1] - mPoints[0],
                                            mPoints[2] - mPoints[0]).abs() + EPS) {
            return result;
        } else {
            return Intersection();
        }
    }
    // RAY -> NONE
    return Intersection();
}

vector<Vector> Triangle::getPoints() const {
    vector<Vector> result;
    for (int i = 0; i < 3; ++i) {
        result.push_back(mPoints[i]);
    }
    return result;
}

Vector Triangle::getNormal(Vector const &point) const {
    return mNormal;
}

Shape* Triangle::clone() const {
    return new Triangle(*this);
}
