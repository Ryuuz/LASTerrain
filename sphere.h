#ifndef SPHERE_H
#define SPHERE_H

#include "sceneobject.h"

class QVector3D;

class Sphere : public SceneObject
{
    public:
        Sphere(int n = 0);
        ~Sphere();

    private:
        void init() override;
        void makeSphere();
        void subdivide(const QVector3D &u, const QVector3D &v, const QVector3D &w, int n);
        void makeTriangle(const QVector3D &u, const QVector3D &v, const QVector3D &w);

        int mRecursions;
        int mIndex;
};

#endif // SPHERE_H
