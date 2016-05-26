#include <iostream>
#include "MyGL.h"

using namespace std;
using namespace MyGL;
using namespace Geometry3d;

Scene::Scene() : mUnits() { }

Scene::Scene(Vector camera, Parallelogram screen,
             pair<int, int> resolution) : mCamera(camera), mScreen(screen),
                                               mResolution(resolution), mUnits() { }

Scene::Unit::Unit() { }

Scene::Unit::Unit(Shape *shape, BoundingBox boundingBox, Color color) :
    mShape(shape), mBoundingBox(boundingBox), mColor(color) { }

void Scene::addUnit(Shape *shape, Color color) {
    mUnits.push_back(Unit(shape, BoundingBox(shape), color));
}

void Scene::addUnit(Geometry3d::Shape const &shape, Color color) {
    mUnits.push_back(Unit(shape.clone(), BoundingBox(&shape), color));
}

void Scene::rayTrace(std::vector<std::vector<Color>> &pixels) {
    mKDTree.build(mUnits);
    mUnits.clear();
    pixels.resize(mResolution.first);
    for (auto &v : pixels) {
        v.resize(mResolution.second);
    }
    auto axis0 = (mScreen.getPoint(1) - mScreen.getPoint(0)) / mResolution.first;
    auto axis1 = (mScreen.getPoint(3) - mScreen.getPoint(0)) / mResolution.second;
    for (int i = 0; i < mResolution.first; ++i) {
        //cerr << ".";
        for (int j = 0; j < mResolution.second; ++j) {
            Ray ray(mCamera, mScreen.getPoint(0) + (i + 0.5) * axis0 + (j + 0.5) * axis1);
            pixels[i][j] = trace(ray);
        }
    }
}

Color Scene::trace(const Ray &ray) const {
    /*Color answer(0, 0, 0); // black
    double minDistance = -1;
    for (auto &unit : mUnits) {
        auto intersection = unit.mShape->intersect(ray);
        if (intersection.mResult == Intersection::POINTS) {
            auto dst = intersection.mPoints[0].distanceTo(mCamera);
            if (minDistance < 0 || dst < minDistance) {
                minDistance = dst;
                answer = unit.mColor;
            }
        }
    }
    return answer;*/
    auto result = mKDTree.trace(mCamera, ray);
    if (result.second < 0) {
        return Color(0, 0, 0); // black
    }
    Color answer(0, 0, 0);
    for (auto &source : mLights) {
        // simplest case
        auto point = result.first->mShape->intersect(ray).mPoints[0];
        answer = answer + result.first->mColor * source.getIntencity(point, result.first->mShape->getNormal());
    }
    return answer;
}
