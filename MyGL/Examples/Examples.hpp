#ifndef GRAPHIC_LIBRARY_EXAMPLES_HPP
#define GRAPHIC_LIBRARY_EXAMPLES_HPP

#include "../MyGL.h"
#include "../../Geometry3d/Sphere.h"
#include <iostream>


#include <bits/stdc++.h>
using namespace std;

namespace MyGL {

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
            redTriangle.addLight(Geometry3d::Vector(2, 0, 8), 100.);

            screen = Geometry3d::Parallelogram(Geometry3d::Vector(80, -5, -5),
                                               Geometry3d::Vector(80, 5, -5),
                                               Geometry3d::Vector(80, 5, 5));

            greenSphere = MyGL::Scene(Geometry3d::Vector(100, 0, 0), screen,
                                      std::make_pair(700, 700), 1);

            fig = (Geometry3d::Shape *)new Geometry3d::Sphere(
                    Geometry3d::Vector(-5, -10, -10), 7.5, Geometry3d::Sphere::MERCATOR);
            auto pic = readBMP("MyGL/Examples/earth.bmp");
            greenSphere.addUnit(fig, pic);

            pic = readBMP("MyGL/Examples/earth2.bmp");
            fig = (Geometry3d::Shape *)new Geometry3d::Sphere(
                    Geometry3d::Vector(5, 10, 10), 7.5);
            greenSphere.addUnit(fig, pic);
            greenSphere.addLight(Geometry3d::Vector(100, 50, 50), 10000.);
        }

        MyGL::Scene redTriangle;
        MyGL::Scene greenSphere;

    //private:

        static std::vector<std::vector<Color>> readBMP(const char *fileName) {
            ifstream fin(fileName, ios::binary|ios::in);

            char *info = new char[28]; // some headers
            int width, height;
            fin.read(info, 18);
            fin.read((char *)&width, 4);
            fin.read((char *)&height, 4);
            fin.read(info, 28);

            int line = width * 3 + width % 4; // the length of line with padding
            char *data = new char[line];

            std::vector<std::vector<Color>> image(width, std::vector<Color>(height));

            for (int i = 0; i < height; i++) {
                fin.read(data, line);
                for (int j = 0; j < width * 3; j += 3) {
                    image[j / 3][i] = Color(data[j + 2], data[j + 1], data[j]);
                }
            }

            delete[] data;
            delete[] info;
            fin.close();

            return image;
        }

    };
}

#endif // GRAPHIC_LIBRARY_EXAMPLES_HPP
