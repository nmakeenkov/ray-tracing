#ifndef GRAPHIC_LIBRARY_GL_H
#define GRAPHIC_LIBRARY_GL_H

#include "../Geometry3d/Geometry3d.h"
#include <vector>
#include <cmath>

namespace MyGL {

    class Color {
    public:
        Color();
        Color(unsigned char red, unsigned char green, unsigned char blue);

        Color operator*(double c) const;
        Color operator+(Color const &other) const;

        double mRed, mGreen, mBlue;
    };


    class Scene {
    public:
        Scene();
        Scene(Geometry3d::Vector camera, Geometry3d::Parallelogram screen,
              std::pair<int, int> resolution, int threads = -1);

        void addUnit(Geometry3d::Shape *shape, Color color, double reflectance = 0, double refractionIndex = 0);
        void addUnit(Geometry3d::Shape const &shape, Color color, double reflectance = 0, double refractionIndex = 0);
        void addUnit(Geometry3d::Shape *shape, std::vector<std::vector<Color>> texture,
                     double reflectance = 0, double refractionIndex = 0);
        void addUnit(Geometry3d::Shape const &shape, std::vector<std::vector<Color>> texture,
                     double reflectance = 0, double refractionIndex = 0);
        void addLight(Geometry3d::Vector const &source, double strength);
        void rayTrace(std::vector<std::vector<Color>> &pixels);

        Geometry3d::Vector mCamera;
        Geometry3d::Parallelogram mScreen;
    private:
        static const int RECTANGLE_COUNT_SQRT = 25;
        static const int MAX_REFLECTION_DEPTH = 4;
        class LightSource {
        public:
            LightSource();
            LightSource(Geometry3d::Vector const &source, double strength);

            double getIntencity(Geometry3d::Vector const &point, Geometry3d::Vector const &normal) const;

            Geometry3d::Vector mSource;
            double mStrength;

        };

        class BoundingBox {
        public:
            BoundingBox();
            BoundingBox(double Xmin, double Xmax,
                        double Ymin, double Ymax,
                        double Zmin, double Zmax);
            BoundingBox(Geometry3d::Shape const *);
            bool intersects(Geometry3d::Ray const &ray) const;

            void updateSides();
            void expand(BoundingBox const &other); // do not forget to update sides after it

            double mXmin, mXmax, mYmin, mYmax, mZmin, mZmax;
            Geometry3d::Parallelogram sides[6];
        };

        class Unit {
        public:
            Unit();
            Unit(Geometry3d::Shape *shape, BoundingBox boundingbox, Color color,
                 double reflectance = 0, double refractionIndex = 0);
            Unit(Geometry3d::Shape *shape, BoundingBox boundingbox, std::vector<std::vector<Color>> texture,
                 double reflectance = 0, double refractionIndex = 0);

            Color getTextureColor(std::pair<double, double> const &point);

            Geometry3d::Shape *mShape;
            BoundingBox mBoundingBox;
            enum { COLOR, TEXTURE } mType;
            Color mColor;
            std::vector<std::vector<Color>> mTexture;
            double mReflectance;
            double mRefractionIndex;
        };

        class KDTree {
        public:
            KDTree();
            void build(std::vector<Unit> &units);
            std::pair<Unit*, double> trace(Geometry3d::Vector camera, Geometry3d::Ray const &) const;
            void operator=(KDTree const &other);

        private:
            const unsigned int MAX_COUNT_IN_LEAF = 10;

            class Node {

            public:
                enum Axes {
                    AXIS_X = 0,
                    AXIS_Y = 1,
                    AXIS_Z = 2
                };
                Axes mSplitAxis;
                unsigned int mCount;
                BoundingBox mBoundingBox;
                Node *mLeft; // <= splitValue
                Node *mRight; // > splitValue
                std::vector<Unit> mUnits; // only in leaves
                Node();
            };

            Node *mRoot;
            void buildRecursively(Node *node, Unit *units);
            std::pair<Unit*, double> traceRecursively(Node *node, Geometry3d::Vector camera,
                                                      Geometry3d::Ray const &ray) const;
        };

        void traceRectangle(Geometry3d::Vector const &axis0, Geometry3d::Vector const &axis1,
                            int xFrom, int xTo, int yFrom, int yTo,
                            std::vector<std::vector<Color>> &pixels);
        Color trace(Geometry3d::Ray const &ray, int curDepth = 0) const;

        std::pair<int, int> mResolution;

        KDTree mKDTree;
        std::vector<Unit> mUnits;
        std::vector<LightSource> mLights;
        int mThreads;
    };

};

#endif // GRAPHIC_LIBRARY_GL_H
