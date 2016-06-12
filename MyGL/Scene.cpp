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
                  double reflectance, double refractionIndex, double refractedPart) :
                      mShape(shape), mBoundingBox(boundingBox), mType(COLOR),
                      mColor(color), mTexture(),
                      mReflectance(reflectance), mRefractionIndex(refractionIndex), mRefractedPart(refractedPart) { }

Scene::Unit::Unit(Shape *shape, BoundingBox boundingBox, vector<std::vector<Color>> texture,
                  double reflectance, double refractionIndex, double refractedPart) :
                      mShape(shape), mBoundingBox(boundingBox), mType(TEXTURE),
                      mColor(), mTexture(texture),
                      mReflectance(reflectance), mRefractionIndex(refractionIndex), mRefractedPart(refractedPart) { }

Color Scene::Unit::getTextureColor(pair<double, double> const &point) {
    return mTexture[int(mTexture.size() * point.first) % mTexture.size()][int(mTexture[0].size() * point.second) % mTexture[0].size()];
}

void Scene::addUnit(Shape *shape, Color color, double reflectance, double refractionIndex, double refractedPart) {
    mUnits.push_back(Unit(shape, BoundingBox(shape), color, reflectance, refractionIndex, refractedPart));
}

void Scene::addUnit(Geometry3d::Shape const &shape, Color color, double reflectance, double refractionIndex, double refractedPart) {
    mUnits.push_back(Unit(shape.clone(), BoundingBox(&shape), color, reflectance, refractionIndex, refractedPart));
}

void Scene::addUnit(Shape *shape, vector<vector<Color>> texture, double reflectance, double refractionIndex, double refractedPart) {
    mUnits.push_back(Unit(shape, BoundingBox(shape), texture, reflectance, refractionIndex, refractedPart));
}

void Scene::addUnit(Geometry3d::Shape const &shape, vector<vector<Color>> texture, double reflectance, double refractionIndex, double refractedPart) {
    mUnits.push_back(Unit(shape.clone(), BoundingBox(&shape), texture, reflectance, refractionIndex, refractedPart));
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
        auto point2d = result.first->mShape->getPoint2d(point);
        myColor = result.first->getTextureColor(point2d);
    }

/*
    cerr << myColor.mRed << " " << myColor.mGreen << " " << myColor.mGreen << endl;
    string a;
    getline(cin, a);
*/
    if (result.first->mRefractionIndex > EPS) {
        // refraction
        auto projection = ray.mDirection - (Vector::scalarProduction(ray.mDirection, normal) *
            normal); // sin a = |projection| / |ray.mDirection|

        /*
        auto vPlus = -1 * ray.mDirection - projection;
        vPlus = vPlus * (-result.first->mRefractionIndex);
        ////////////
        auto projectionRefracted = result.first->mRefractionIndex *
            projection; // now only have to make the length of newDirection the same
                        // as ray.mDirection has

        double multiplier = -sqrt(Vector::scalarProduction(ray.mDirection,
                                                          ray.mDirection) -
                                 Vector::scalarProduction(projectionRefracted,
                                                          projectionRefracted));

        auto newDirection = (normal * multiplier) + projectionRefracted;
        */
        double t = (result.first->mRefractionIndex * ray.mDirection.abs());
        double p = t * t - Utils::sqr(projection.abs());
        if (p > 0) {
            p = sqrt(p);
        } else {
            p = 0;
        }
        if (Vector::scalarProduction(ray.mDirection, normal) < 0) {
            p *= -1;
        }
        auto newDirection = p * normal + projection;
/*
        //cerr << multiplier << " " << abs(Vector::scalarProduction(ray.mDirection, normal)) << endl;
        //cerr << projection.abs() << " " << ray.mDirection.abs() << "   " << newDirection.abs() << endl;
        cerr << "__ " << ray.mDirection.mX << " " << ray.mDirection.mY << " " << ray.mDirection.mZ << endl;
        cerr << normal.mX << " " << normal.mY << " " << normal.mZ << endl;
        cerr << point.mX << " " << point.mY << " " << point.mZ << endl;
        cerr << "__ " << newDirection.mX << " " << newDirection.mY << " " << newDirection.mZ << endl;

        string s;
        getline(cin, s);
*/
        answer = trace(Ray(point + newDirection * EPS, point + newDirection), curDepth + 1) * result.first->mRefractedPart;
    }
    if (result.first->mRefractionIndex < EPS || result.first->mRefractedPart < 1) {
        // TODO: 0.2 as a param
        answer = answer + myColor * 0.2 * (result.first->mRefractionIndex > EPS ? 1 - result.first->mRefractedPart : 1);
        double summaryLight = 0;
        for (auto &source : mLights) {
            Ray checkRay(source.mSource, point);
            auto check = mKDTree.trace(source.mSource, checkRay);
            if (check.first != result.first) {
                continue;
            }
            if ((check.first->mShape->intersect(checkRay).mPoints[0] - point).abs() > EPS) {
                continue;
            }
            summaryLight += source.getIntencity(point, normal);
        }
        if (result.first->mRefractionIndex > EPS) {
            summaryLight *= 1 - result.first->mRefractedPart;
        }
        answer = answer + myColor * summaryLight;
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
