#include "Sphere.h"

using namespace Geometry3d;

Sphere::Sphere() { }

Sphere::Sphere(Vector const &center, double radius) : mCenter(center), mRadius(radius) { }