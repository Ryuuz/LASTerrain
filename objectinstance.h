#ifndef OBJECTINSTANCE_H
#define OBJECTINSTANCE_H

#include <QOpenGLFunctions_4_1_Core>
#include "boundingvolume.h"

class SceneObject;
class QVector3D;
class QMatrix4x4;
class Camera;
class Transform;


class ObjectInstance : protected QOpenGLFunctions_4_1_Core
{
    public:
        ObjectInstance(SceneObject *a = nullptr);
        ~ObjectInstance();
        void draw();

        void setBoundingObject(BoundType type);
        void setBoundingObject(QVector3D max, QVector3D min, BoundType type);
        void setColor(QVector3D color) {mObjectColor = color;}
        void setTag(std::string tag) {mTag = tag;}
        void setDestroyed(bool state) {mDestroyed = state;}

        BoundingVolume* getBounds() const {return mBoundingObject;}
        Transform* getTransform() const {return mTransform;}
        QMatrix4x4* getModelMatrix();
        std::string getTag() const {return mTag;}
        SceneObject* getModel() const {return mModel;}
        QVector3D getPosition();
        bool isDestroyed() const {return mDestroyed;}

    protected:
        SceneObject *mModel;
        QMatrix4x4 *mModelMatrix;
        BoundingVolume *mBoundingObject;
        Transform *mTransform;
        QVector3D mObjectColor;
        std::string mTag;
        bool mDestroyed = false;
};

#endif // OBJECTINSTANCE_H
