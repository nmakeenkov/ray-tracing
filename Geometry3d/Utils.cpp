#include "Utils.h"

using namespace Geometry3d;

int Utils::sign(double a) {
    if (a > 0) {
        return 1;
    }
    if (a < 0) {
        return -1;
    }
    return 0;
}

double Utils::sqr(double x) {
    return x * x;
}
