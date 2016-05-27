#include <iostream>
#include "MyGL.h"
#include "../MultiThreading/MultiThreading.h"

using namespace std;
using namespace MyGL;
using namespace Geometry3d;

Scene::Scene() : mUnits() { }

Scene::Scene(Vector camera, Parallelogram screen,
             pair<int, int> resolution, int threads) : mCamera(camera), mScreen(screen),
                                               mResolution(resolution), mUnits(), mThreads(threads) { }

Scene::Unit::Unit() { }

Scene::Unit::Unit(Shape *shape, BoundingBox boundingBox, Color color, double reflectance) :
    mShape(shape), mBoundingBox(boundingBox), mColor(color), mReflectance(reflectance) { }

void Scene::addUnit(Shape *shape, Color color, double reflectance) {
    mUnits.push_back(Unit(shape, BoundingBox(shape), color, reflectance));
}

void Scene::addUnit(Geometry3d::Shape const &shape, Color color, double reflectance) {
    mUnits.push_back(Unit(shape.clone(), BoundingBox(&shape), color, reflectance));
}

void Scene::traceRectangle(Vector const &axis0, Vector const &axis1,
                           int xFrom, int xTo, int yFrom, int yTo, vector<vector<Color>> &pixels) {
    if (mResolution.first < xTo) {
        xTo = mResolution.first;
    }
    if (mResolution.second < yTo) {
        yTo = mResolution.second;
    }
    for (int i = xFrom; i < xTo; ++i) {
        for (int j = yFrom; j < yTo; ++j) {
            Ray ray(mCamera, mScreen.getPoint(0) + (i + 0.5) * axis0 + (j + 0.5) * axis1);
            pixels[i][j] = trace(ray);
        }
    }
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
    if (mThreads == 1) {
        for (int i = 0; i < mResolution.first; ++i) {
            for (int j = 0; j < mResolution.second; ++j) {
                Ray ray(mCamera, mScreen.getPoint(0) + (i + 0.5) * axis0 + (j + 0.5) * axis1);
                pixels[i][j] = trace(ray);
            }
        }
    } else {
        MultiThreading::ThreadPool threadPool(mThreads);
        int stepX = mResolution.first / RECTANGLE_COUNT_SQRT;
        int stepY = mResolution.second / RECTANGLE_COUNT_SQRT;
        for (int i = 0; i < mResolution.first; i += stepX) {
            for (int j = 0; j < mResolution.second; j += stepY) {
                threadPool.addTask(std::bind(&Scene::traceRectangle, this, std::ref(axis0), std::ref(axis1),
                                             i, i + stepX, j, j + stepY, std::ref(pixels)));
            }
        }
    }
}

Color Scene::trace(const Ray &ray, int curDepth) const {
    auto result = mKDTree.trace(mCamera, ray);
    if (result.second < 0) {
        return Color(0, 0, 0); // black
    }
    Color answer(0, 0, 0);
    auto point = result.first->mShape->intersect(ray).mPoints[0];
    auto normal = result.first->mShape->getNormal(point);
    for (auto &source : mLights) {
        Ray checkRay(source.mSource, point);
        auto check = mKDTree.trace(source.mSource, checkRay);
        if (check.first != result.first) {
            continue;
        }
        if ((check.first->mShape->intersect(checkRay).mPoints[0] - point).abs() > EPS) {
            continue;
        }
        answer = answer + result.first->mColor * source.getIntencity(point, normal);
    }
    if (result.first->mReflectance > 0 && curDepth < MAX_REFLECTION_DEPTH) {
        Vector newPoint(point + ray.mDirection - 2. * Vector::scalarProduction(normal, ray.mDirection) * normal);
        answer = answer * (1. - result.first->mReflectance)
            + trace(Ray(point, newPoint), curDepth + 1) * result.first->mReflectance;
    }
    return answer;
}
