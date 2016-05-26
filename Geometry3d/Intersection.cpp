#include "Intersection.h"

using namespace std;
using namespace Geometry3d;

Intersection::Intersection() : mResult(NONE) { }

Intersection::Intersection(std::vector<Vector> const &points) : mResult(POINTS), mPoints(points) { }

Intersection::Intersection(Ray *ray) : mResult(RAY), mRay(ray) { }