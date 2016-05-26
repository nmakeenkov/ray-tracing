#include "Parallelogram.h"

using namespace std;
using namespace Geometry3d;

Parallelogram::Parallelogram() : Shape() { }

Parallelogram::Parallelogram(const Vector &f, const Vector &s, const Vector &t) : Shape(), mPlane(f, s, t) {
    mPoints[0] = f;
    mPoints[1] = s;
    mPoints[2] = t;
    mPoints[3] = t + f - s;
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
