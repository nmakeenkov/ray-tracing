#include "MyGL.h"
#include <algorithm>
#include <iostream>

using namespace MyGL;
using namespace Geometry3d;
using namespace std;

Scene::BoundingBox::BoundingBox() { }

Scene::BoundingBox::BoundingBox(double Xmin, double Xmax,
                                double Ymin, double Ymax,
                                double Zmin, double Zmax) :
                                    mXmin(Xmin), mXmax(Xmax),
                                    mYmin(Ymin), mYmax(Ymax),
                                    mZmin(Zmin), mZmax(Zmax) {
    updateSides();
}

void Scene::BoundingBox::updateSides() {
    sides[0] = Parallelogram(Vector(mXmin, mYmin, mZmin),
                      Vector(mXmin, mYmax, mZmin),
                      Vector(mXmin, mYmax, mZmax));
    sides[1] = Parallelogram(Vector(mXmax, mYmin, mZmin),
                      Vector(mXmax, mYmax, mZmin),
                      Vector(mXmax, mYmax, mZmax));
    sides[2] = Parallelogram(Vector(mXmin, mYmin, mZmin),
                      Vector(mXmax, mYmin, mZmin),
                      Vector(mXmax, mYmin, mZmax));
    sides[3] = Parallelogram(Vector(mXmin, mYmax, mZmin),
                      Vector(mXmax, mYmax, mZmin),
                      Vector(mXmax, mYmax, mZmax));
    sides[4] = Parallelogram(Vector(mXmin, mYmin, mZmin),
                      Vector(mXmin, mYmax, mZmin),
                      Vector(mXmax, mYmax, mZmin));
    sides[5] = Parallelogram(Vector(mXmin, mYmin, mZmax),
                      Vector(mXmin, mYmax, mZmax),
                      Vector(mXmax, mYmax, mZmax));
}

Scene::BoundingBox::BoundingBox(Geometry3d::Shape const *shape) {
    auto points = shape->getPoints();
    mXmin = mXmax = points[0].mX;
    mYmin = mYmax = points[0].mY;
    mZmin = mZmax = points[0].mZ;
    for (auto &point : points) {
        if (point.mX < mXmin) {
            mXmin = point.mX;
        }
        if (point.mX > mXmax) {
            mXmax = point.mX;
        }
        if (point.mY < mYmin) {
            mYmin = point.mY;
        }
        if (point.mY > mYmax) {
            mYmax = point.mY;
        }
        if (point.mZ < mZmin) {
            mZmin = point.mZ;
        }
        if (point.mZ > mZmax) {
            mZmax = point.mZ;
        }
    }
}

bool Scene::BoundingBox::intersects(Geometry3d::Ray const &ray) const {
    for (int i = 0; i < 6; ++i) {
        if (sides[i].intersect(ray).mResult != Intersection::NONE) {
            return true;
        }
    }
    return false;
}

void Scene::BoundingBox::expand(BoundingBox const &other) {
    if (other.mXmin < mXmin) {
        mXmin = other.mXmin;
    }
    if (other.mXmax > mXmax) {
        mXmax = other.mXmax;
    }
    if (other.mYmin < mYmin) {
        mYmin = other.mYmin;
    }
    if (other.mYmax > mYmax) {
        mYmax = other.mYmax;
    }
    if (other.mZmin < mZmin) {
        mZmin = other.mZmin;
    }
    if (other.mZmax > mZmax) {
        mZmax = other.mZmax;
    }
}

Scene::KDTree::Node::Node() : mLeft(nullptr), mRight(nullptr) { }

Scene::KDTree::KDTree() : mRoot(nullptr) { }

void Scene::KDTree::build(vector<Unit> &units) {
    if (mRoot != nullptr) {
        delete mRoot;
    }
    mRoot = new Node();
    mRoot->mSplitAxis = Node::AXIS_X;
    mRoot->mCount = units.size();
    buildRecursively(mRoot, &units[0]);
    /*
    cerr << mRoot->mBoundingBox.mXmin << " " << mRoot->mBoundingBox.mXmax << "\n"
        << mRoot->mBoundingBox.mYmin << " " << mRoot->mBoundingBox.mYmax << "\n"
        << mRoot->mBoundingBox.mZmin << " " << mRoot->mBoundingBox.mZmax << endl;
        */
}

void Scene::KDTree::buildRecursively(Node *node, Unit *units) {
    if (node->mCount <= MAX_COUNT_IN_LEAF) {
        node->mUnits = vector<Unit>(units, units + node->mCount);
        node->mBoundingBox = node->mUnits[0].mBoundingBox;
        for (auto &unit : node->mUnits) {
            node->mBoundingBox.expand(unit.mBoundingBox);
        }
        node->mBoundingBox.updateSides();
        return;
    }

    switch (node->mSplitAxis) {
        case Node::AXIS_X:
            nth_element(units, units + node->mCount / 2,
                        units + node->mCount,
                        [](Unit const &less, Unit const &more) {
                            return less.mBoundingBox.mXmin < more.mBoundingBox.mXmin;
                        });
            break;

        case Node::AXIS_Y:
            nth_element(units, units + node->mCount / 2,
                        units + node->mCount,
                        [](Unit const &less, Unit const &more) {
                            return less.mBoundingBox.mYmin < more.mBoundingBox.mYmin;
                        });
            break;

        case Node::AXIS_Z:
            nth_element(units, units + node->mCount / 2,
                        units + node->mCount,
                        [](Unit const &less, Unit const &more) {
                            return less.mBoundingBox.mZmin < more.mBoundingBox.mZmin;
                        });
    }

    node->mLeft = new Node();
    node->mLeft->mSplitAxis = (Node::Axes)((node->mSplitAxis + 1) % 3);
    node->mLeft->mCount = node->mCount / 2;
    buildRecursively(node->mLeft, units);

    node->mRight = new Node();
    node->mRight->mSplitAxis = (Node::Axes)((node->mSplitAxis + 1) % 3);
    node->mRight->mCount = node->mCount - node->mLeft->mCount;
    buildRecursively(node->mRight, units + node->mLeft->mCount);

    node->mBoundingBox = BoundingBox(min(node->mLeft->mBoundingBox.mXmin,
                                         node->mRight->mBoundingBox.mXmin),
                                     max(node->mLeft->mBoundingBox.mXmax,
                                         node->mRight->mBoundingBox.mXmax),
                                     min(node->mLeft->mBoundingBox.mYmin,
                                         node->mRight->mBoundingBox.mYmin),
                                     max(node->mLeft->mBoundingBox.mYmax,
                                         node->mRight->mBoundingBox.mYmax),
                                     min(node->mLeft->mBoundingBox.mZmin,
                                         node->mRight->mBoundingBox.mZmin),
                                     max(node->mLeft->mBoundingBox.mZmax,
                                         node->mRight->mBoundingBox.mZmax));
}

pair<Scene::Unit*, double> Scene::KDTree::trace(Geometry3d::Vector camera, Geometry3d::Ray const &ray) const {
    return traceRecursively(mRoot, camera, ray);
}

pair<Scene::Unit*, double> Scene::KDTree::traceRecursively(Node *node, Geometry3d::Vector camera,
                                                    Geometry3d::Ray const &ray) const {
    if (!node->mBoundingBox.intersects(ray)) {
        return make_pair(nullptr, -1);
    }
    if (!node->mLeft) {
        Unit *answer = nullptr;
        double minDistance = -1;
        for (auto &unit : node->mUnits) {
            auto intersection = unit.mShape->intersect(ray);
            if (intersection.mResult == Geometry3d::Intersection::POINTS) {
                auto dst = intersection.mPoints[0].distanceTo(camera);
                if (minDistance < 0 || dst < minDistance) {
                    minDistance = dst;
                    answer = &unit;
                }
            }
        }
        return make_pair(answer, minDistance);
    }

    auto answerLeft = traceRecursively(node->mLeft, camera, ray);
    auto answerRight = traceRecursively(node->mRight, camera, ray);
    if (answerLeft.second < 0) {
        return answerRight;
    }
    if (answerRight.second < 0 || answerRight.second > answerLeft.second) {
        return answerLeft;
    }
    return answerRight;
}

void Scene::KDTree::operator=(KDTree const &other) {
    // TODO: copy, as it may be deleted
    if (mRoot != nullptr) {
        delete mRoot;
    }
    mRoot = other.mRoot;
}
