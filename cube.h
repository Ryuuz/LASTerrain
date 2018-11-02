#ifndef CUBE_H
#define CUBE_H

#include "sceneobject.h"
#include <QVector3D>

class Cube : public SceneObject
{
    public:
        Cube();
        ~Cube();

    private:
        void init() override;
};

#endif // CUBE_H
