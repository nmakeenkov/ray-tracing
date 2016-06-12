#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include "MyGL/MyGL.h"
#include "MyGL/Examples/Examples.hpp"
#include "MyGL/Readers/ReadSTL.h"

using namespace std;
using namespace MyGL;

vector<vector<Color>> pixels;

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    for (int i = 0; i < 700; ++i) {
        for (int j = 0; j < 700; ++j) {
            glColor3f(pixels[i][j].mRed / 255.0, pixels[i][j].mGreen / 255.0, pixels[i][j].mBlue / 255.0);
            glVertex2d(i, j);
        }
    }
    glEnd();
    glFlush();
}

void Initialize() {
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 700.0, 0.0, 700.0, -1.0, 1.0);
}

int main(int argc, char **argv) {

    auto scene = Readers::ReadSTL::read("MyGL/Examples/xBowl.stl");
    //auto scene = Readers::ReadSTL::read("MyGL/Examples/monster.stl");
    //auto scene = Examples().redTriangle;
    //auto scene = Examples().greenSphere;
    scene.rayTrace(pixels);

    /*
    auto pic = Examples::readBMP("MyGL/Examples/water.bmp");
    for (int i = 0; i < 700; ++i) {
        for (int j = 0; j < 700; ++j) {
            pixels[i][j] = pic[i * 0.7][j * 0.7];
        }
    } */
    //return 0;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("My super elite graphic library!!1337");
    Initialize();
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
}
