#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "objectinstance.h"
#include <QVector3D>

class QKeyEvent;
class PointCloud;

class PhysicsObject : public ObjectInstance
{
    public:
        PhysicsObject(SceneObject *a);
        ~PhysicsObject();

        virtual void physicsUpdate(float deltaTime);
        virtual void moveObject();
        void handleInput(QKeyEvent *event, bool pressed);
        void setVelocity(float deltaTime);
        void setVelocity(QVector3D vel);
        void setYPos(float posY);
        QVector3D getVelocity() {return mVelocity;}

    protected:
        QVector3D mVelocity;
        float mFriction;
        float mSpeed;
        float mProgress;
        bool mReturning;

        bool mWPressed = false;
        bool mAPressed = false;
        bool mSPressed = false;
        bool mDPressed = false;
};

#endif // PHYSICSOBJECT_H
