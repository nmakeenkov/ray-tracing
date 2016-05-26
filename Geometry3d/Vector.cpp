#include "Vector.h"
#include <cmath>

using namespace Geometry3d;

Vector::Vector() { }

Vector::Vector(double x, double y, double z) : mX(x), mY(y), mZ(z) { }

Vector::Vector(const Vector &other) : mX(other.mX), mY(other.mY), mZ(other.mZ) { }

double Vector::scalarProduction(const Vector &first, const Vector &second) {
    return first.mX * second.mX + first.mY * second.mY + first.mZ * second.mZ;
}

Vector Vector::vectorProduction(const Vector &first, const Vector &second) {
    return Vector(first.mY * second.mZ - first.mZ * second.mY,
                  -first.mX * second.mZ + first.mZ * second.mX,
                  first.mX * second.mY - first.mY * second.mX);
}

double Vector::abs() const {
    return sqrt(scalarProduction(*this, *this));
}

double Vector::distanceTo(const Vector &other) const {
    return (other - *this).abs();
}

Vector Vector::operator+(const Vector &other) const {
    return Vector(mX + other.mX, mY + other.mY, mZ + other.mZ);
}

Vector Vector::operator-(const Vector &other) const {
    return Vector(mX - other.mX, mY - other.mY, mZ - other.mZ);
}

Vector Vector::operator*(double c) const {
    return Vector(c * mX, c * mY, c * mZ);
}

Geometry3d::Vector operator*(double c, const Geometry3d::Vector &v) {
    return v * c;
}

Vector Vector::operator/(double c) const {
    return Vector(mX / c, mY / c, mZ / c);
}

bool Vector::operator==(const Vector &other) const {
    return fabs(mX - other.mX) < EPS && fabs(mY - other.mY) < EPS && fabs(mZ - other.mZ) < EPS;
}

bool Vector::operator!=(const Vector &other) const {
    return fabs(mX - other.mX) > EPS || fabs(mY - other.mY) > EPS || fabs(mZ - other.mZ) > EPS;
}
