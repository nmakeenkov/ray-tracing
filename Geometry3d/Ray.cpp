#include "Ray.h"

using namespace Geometry3d;

Ray::Ray() { }

Ray::Ray(Vector const &start, Vector const &point) : mStart(start), mDirection(point - start) { }