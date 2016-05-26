#ifndef GRAPHIC_LIBRARY_EXAMPLES_HPP
#define GRAPHIC_LIBRARY_EXAMPLES_HPP

#include "../MyGL.h"
#include <iostream>

class Examples {
public:
    Examples() {
        Geometry3d::Vector zero(0, 0, 0);
        Geometry3d::Parallelogram screen(Geometry3d::Vector(1, 0, 0),
                                         Geometry3d::Vector(0, 1, 0),
                                         Geometry3d::Vector(0, 1, 1));
        MyGL::Color red(200, 0, 0);
        redTriangle = MyGL::Scene(zero, screen, std::make_pair(700, 700));
        Geometry3d::Shape *fig = (Geometry3d::Shape *)new Geometry3d::Triangle(
                Geometry3d::Vector(10, 0, 1),
                Geometry3d::Vector(0, 10, 0.3),
                Geometry3d::Vector(5, 5, 5));
        redTriangle.addUnit(fig, red);
    }

    MyGL::Scene redTriangle;
};

#endif // GRAPHIC_LIBRARY_EXAMPLES_HPP