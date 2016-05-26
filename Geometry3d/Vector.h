#ifndef GRAPHIC_LIBRARY_GEOMETRY3D_VECTOR_H
#define GRAPHIC_LIBRARY_GEOMETRY3D_VECTOR_H

namespace Geometry3d {

    const double EPS = 1e-6;

    class Vector {
    public:
        Vector();
        Vector(double x, double y, double z);
        Vector(Vector const &);

        static double scalarProduction(Vector const &, Vector const &);
        static Vector vectorProduction(Vector const &, Vector const &);
        double abs() const;
        double distanceTo(Vector const &) const;
        Vector operator+(Vector const &) const;
        Vector operator-(Vector const &) const;
        Vector operator*(double) const;
        Vector operator/(double) const;
        bool operator==(Vector const &) const;
        bool operator!=(Vector const &) const;

        double mX, mY, mZ;
    };

}

Geometry3d::Vector operator*(double c, const Geometry3d::Vector &v);

#endif // GRAPHIC_LIBRARY_GEOMETRY3D_VECTOR_H