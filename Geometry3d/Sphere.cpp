#include "Sphere.h"

using namespace Geometry3d;
using namespace std;

Sphere::Sphere() { }

Sphere::Sphere(Vector const &center, double radius, Projection projection) : mCenter(center),
                                                                             mRadius(radius),
                                                                             mProjection(projection) { }

Intersection Sphere::intersect(Ray const &ray) const {
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

vector<Vector> Sphere::getPoints() const {
    vector<Vector> result;
    // for bounding box
    result.push_back(mCenter + Vector(mRadius, mRadius, mRadius));
    result.push_back(mCenter - Vector(mRadius, mRadius, mRadius));
    return result;
}

Vector Sphere::getNormal(Vector const &point) const {
    auto ans = point - mCenter;
    return ans / ans.abs();
}

Shape* Sphere::clone() const {
    return new Sphere(*this);
}

constexpr double calcPI() {
    return acos(-1);
}

const double PI = calcPI();

std::pair<double, double> Sphere::getPoint2d(Vector const &point3d) const {
    // move center to (0, 0, 0)
    auto point = point3d - mCenter;

    // spherical coords
    double lambda = atan2(point.mY, point.mX);
    double phi = PI / 2. - atan2(sqrtl(Utils::sqr(point.mX) + Utils::sqr(point.mY)), point.mZ);

    double x = (lambda + PI) / (2. * PI);
    double y;
    if (mProjection == MERCATOR) {
        y = log(tan(phi / 2. + PI / 4.));
        y = max(0., min(1., (y + 3.) / 6.));
    } else {
        // EQUIRECTANGULAR
        y = (phi + PI / 2.) / PI;
    }
    return make_pair(x, y);
}
