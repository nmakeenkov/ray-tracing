#include "ReadSTL.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "../../Geometry3d/Sphere.h"

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
    Scene scene(Vector(-100, 75, 100),
                Parallelogram(Vector(0, 0, 0),
                              Vector(0, 150, 0),
                              Vector(0, 150, 150)),
                make_pair(700, 700));
    scene.addLight(Vector(0, 0, 200), 100000.);
    Color blue(0, 0, 255);
    ifstream fin(fileName);
    string command;
    float x, y, z;
    int cnt = 0;
    while (fin >> command) {
        if (command == "facet") {
            cnt++;
            if (cnt > 10000) {
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
            scene.addUnit((Shape *)new Triangle(v[0], v[1], v[2]), blue);
        }
    }
    cerr << cnt << endl;
    fin.close();

    Geometry3d::Shape *fig = (Geometry3d::Shape *)new Geometry3d::Sphere(
           Geometry3d::Vector(15, 75, 70), 10);
    scene.addUnit(fig, MyGL::Color(200, 0, 0), 0, 2);

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
    char *tmp = new char[100];
    fin.read(tmp, 80);
    unsigned int triangleCount;
    fin.read((char *)&triangleCount, 4);
    //triangleCount = 800;
    cerr << triangleCount << endl;
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
    delete tmp;
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
