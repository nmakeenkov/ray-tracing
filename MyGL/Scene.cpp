#include <iostream>
#include "MyGL.h"
#include "../MultiThreading/MultiThreading.h"
#include "../Geometry3d/Utils.h"

using namespace std;
using namespace MyGL;
using namespace Geometry3d;

Scene::Scene() : mUnits() { }

Scene::Scene(Vector camera, Parallelogram screen,
             pair<int, int> resolution, int threads) : mCamera(camera), mScreen(screen),
                                               mResolution(resolution), mUnits(), mThreads(threads) { }

Scene::Unit::Unit() { }

Scene::Unit::Unit(Shape *shape, BoundingBox boundingBox, Color color,
                  double reflectance, double refractionIndex) :
                      mShape(shape), mBoundingBox(boundingBox), mType(COLOR),
                      mColor(color), mTexture(),
                      mReflectance(reflectance), mRefractionIndex(refractionIndex) { }

Scene::Unit::Unit(Shape *shape, BoundingBox boundingBox, vector<std::vector<Color>> texture,
                  double reflectance, double refractionIndex) :
                      mShape(shape), mBoundingBox(boundingBox), mType(TEXTURE),
                      mColor(), mTexture(texture),
                      mReflectance(reflectance), mRefractionIndex(refractionIndex) { }

Color Scene::Unit::getTextureColor(pair<double, double> const &point) {
    return mTexture[mTexture.size() * point.first][mTexture[0].size() * point.second];
}

void Scene::addUnit(Shape *shape, Color color, double reflectance, double refractionIndex) {
    mUnits.push_back(Unit(shape, BoundingBox(shape), color, reflectance, refractionIndex));
}

void Scene::addUnit(Geometry3d::Shape const &shape, Color color, double reflectance, double refractionIndex) {
    mUnits.push_back(Unit(shape.clone(), BoundingBox(&shape), color, reflectance, refractionIndex));
}

void Scene::addUnit(Shape *shape, vector<vector<Color>> texture, double reflectance, double refractionIndex) {
    mUnits.push_back(Unit(shape, BoundingBox(shape), texture, reflectance, refractionIndex));
}

void Scene::addUnit(Geometry3d::Shape const &shape, vector<vector<Color>> texture, double reflectance, double refractionIndex) {
    mUnits.push_back(Unit(shape.clone(), BoundingBox(&shape), texture, reflectance, refractionIndex));
}

void Scene::addLight(Geometry3d::Vector const &source, double strength) {
    mLights.push_back(LightSource(source, strength));
}

void Scene::traceRectangle(Vector const &axis0, Vector const &axis1,
                           int xFrom, int xTo, int yFrom, int yTo, vector<vector<Color>> &pixels) {
    if (mResolution.first < xTo) {
        xTo = mResolution.first;
    }
    if (mResolution.second < yTo) {
        yTo = mResolution.second;
    }
        for (int j = yFrom; j < yTo; ++j) {
    for (int i = xFrom; i < xTo; ++i) {
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
        traceRectangle(axis0, axis1, 0, mResolution.first, 0, mResolution.second, pixels);
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

#include "../Geometry3d/Sphere.h"

Color Scene::trace(const Ray &ray, int curDepth) const {
    auto result = mKDTree.trace(mCamera, ray);
    if (result.second < 0) {
        return Color(0, 0, 0); // black
    }
    Color answer(0, 0, 0);
    auto myColor = result.first->mColor;

    auto point = result.first->mShape->intersect(ray).mPoints[0];
    auto normal = result.first->mShape->getNormal(point);

    if (result.first->mType == Unit::TEXTURE) {
        auto kek = (Sphere *)result.first->mShape;
        auto mda = kek->getPoint2d(point);

        myColor = result.first->getTextureColor(mda);
    }

    if (result.first->mRefractionIndex > 0) {
        // refraction
        auto proj = Vector::scalarProduction(-1 * ray.mDirection, normal) * normal;
        auto vPlus = -1 * ray.mDirection - proj;
        vPlus = vPlus * (-result.first->mRefractionIndex);
        auto newDirection = -1 * proj + vPlus;
        answer = trace(Ray(point + newDirection * EPS, point + newDirection), curDepth + 1);
    } else {
        // TODO: plane lights
        answer = answer + myColor * 0.2;
        for (auto &source : mLights) {
            Ray checkRay(source.mSource, point);
            auto check = mKDTree.trace(source.mSource, checkRay);
            if (check.first != result.first) {
                continue;
            }
            if ((check.first->mShape->intersect(checkRay).mPoints[0] - point).abs() > EPS) {
                continue;
            }
            answer = answer + myColor * source.getIntencity(point, normal);
        }
    }
    // reflection
    if (result.first->mReflectance > EPS && curDepth < MAX_REFLECTION_DEPTH) {
        Vector newPoint(point + ray.mDirection - 2. * Vector::scalarProduction(normal, ray.mDirection) * normal);
        answer = answer * (1. - result.first->mReflectance)
            + trace(Ray(point + (newPoint - point) * EPS, newPoint), curDepth + 1) * result.first->mReflectance;
    }
    return answer;
}

Scene::LightSource::LightSource() { }

Scene::LightSource::LightSource(Geometry3d::Vector const &source, double strength) : mSource(source), mStrength(strength) { }

double Scene::LightSource::getIntencity(Geometry3d::Vector const &point, Geometry3d::Vector const &normal) const {
    return fabs(mStrength * (Geometry3d::Vector::scalarProduction(normal, mSource - point))
                / pow(point.distanceTo(mSource), 3.)); // 3, as there's an extra multiplier
                                                       // from scalar production
}
