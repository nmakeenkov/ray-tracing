#include "ReadSTL.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "../MyGL.h"

#include "../../Geometry3d/Sphere.h"
#include "../Examples/Examples.hpp"

using namespace std;
using namespace MyGL;
using namespace Readers;
using namespace Geometry3d;

Scene ReadSTL::read(const char *fileName) {
    ifstream fin(fileName, ios::binary|ios::in);
    string name;
    fin >> name;
    fin.close();
    if (name == "solid") {
        return readASCII(fileName);
    }
    return readBinary(fileName);
}

Scene ReadSTL::readASCII(const char *fileName) {
    Scene scene(Vector(-100, 120, 160),
                Parallelogram(Vector(0, 0, 0),
                              Vector(0, 150, 0),
                              Vector(0, 150, 150)),
                make_pair(700, 700));

    int mxCnt = 10000;

    /*
    auto x_ = Vector(-10, -1, 0);
    auto y_ = Vector(-1, 0, -1);
    double c_ = 0.7;
    x_ = x_ * c_ / x_.abs();
    y_ = y_ * c_ / y_.abs();
    Vector v0(-10, 200, 80);
    Parallelogram screen(v0 + x_ + y_,
                         v0 - x_ + y_,
                         v0 - x_ - y_);

    Vector cam(0, 0, 0);
    for (int i = 0; i < 4; ++i) {
        cam = cam + 0.25 * screen.getPoint(i);
    }
    cerr << cam.mX << " " << cam.mY << " " << cam.mZ << endl; // == v0
    Vector normal = screen.getNormal(cam);
    cam = cam - normal;

    Scene scene(cam,
                screen,
                make_pair(700, 700));
    */


    scene.addLight(Vector(0, 0, 500), 400000.0);
    Color green(0, 255, 0);
    ifstream fin(fileName);
    string command;
    float x, y, z;
    int cnt = 0;

    Scene::BoundingBox bb;

    while (fin >> command) {
        if (command == "facet") {
            cnt++;
            if (cnt > mxCnt) {
                break;
            }
            fin >> command; // normal
            fin >> x >> y >> z;
            fin >> command >> command; // outer loop
            Vector v[3];
            for (int i = 0; i < 3; ++i) {
                fin >> command; // vertex
                fin >> x >> y >> z;
                v[i] = Vector(x, y, z);
            }
            if (cnt < 10) {
                bb = Scene::BoundingBox((Geometry3d::Shape *)new Triangle(v[0], v[1], v[2]));
            }
            bb.expand((Geometry3d::Shape *)new Triangle(v[0], v[1], v[2]));
            scene.addUnit((Shape *)new Triangle(v[0], v[1], v[2]), green);
        }
    }
    bb.updateSides();
    cout << bb.mXmin << " " << bb.mXmax << "\n" <<
        bb.mYmin << " " << bb.mYmax << "\n" <<
        bb.mZmin << " " << bb.mZmax << endl;
    fin.close();

    Geometry3d::Shape *fig = (Geometry3d::Shape *)new Geometry3d::Parallelogram(
           Vector(-350, -350, 40), Vector(-350, 350, 40), Vector(350, 350, 40));

    auto pic = Examples::readBMP("MyGL/Examples/water.bmp");
    cerr << pic.size() << endl << pic[0].size() << endl;
    scene.addUnit(fig, pic, 0, 2, 0.5);

    return scene;
}

MyGL::Scene ReadSTL::readBinary(const char *fileName) {
    Scene scene(Vector(-100, 0, 0),
                Parallelogram(Vector(-50, -50, -50),
                              Vector(-50, 50, -50),
                              Vector(-50, 50, 50)),
                make_pair(700, 700), 1);
    scene.addLight(Vector(-100, 0, 100), 60000.);
    scene.addLight(Vector(1000, 0, 100), 100000.);
    ifstream fin(fileName, ios::binary|ios::in);
    char *tmp = new char[80];
    fin.read(tmp, 80);
    unsigned int triangleCount;
    fin.read((char *)&triangleCount, 4);
    float x, y, z;
    unsigned int attribute;
    for (int i = 0; i < (int)triangleCount; ++i) {
        // normal
        for (int j = 0; j < 3; ++j) {
            fin.read((char *)&x, 4);
        }
        // triangle vertices
        Vector triangle[3];
        for (int j = 0; j < 3; ++j) {
            fin.read((char *)&x, 4);
            fin.read((char *)&y, 4);
            fin.read((char *)&z, 4);
            triangle[j] = Vector(x, y, z);
        }
        scene.addUnit((Shape *)new Triangle(triangle[0], triangle[1], triangle[2]), Color(0, 150, 0));
        fin.read((char *)&attribute, 2);
    }
    delete[] tmp;
    fin.close();

    /*
    Geometry3d::Shape *fig = (Geometry3d::Shape *)new Geometry3d::Sphere(
           Geometry3d::Vector(0, 50, -50), 25);
    */
    Geometry3d::Shape *fig = (Geometry3d::Shape *)
    new Geometry3d::Triangle(Vector(-50, -50, -50), Vector(1500, 50, -50), Vector(-50, 50, -50));

    scene.addUnit(fig, MyGL::Color(0, 0, 200), 0.8);

    return scene;
}
