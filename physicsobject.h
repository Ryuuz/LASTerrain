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
        virtual void moveObject(float deltaTime);
        void addForce(QVector3D force);
        virtual void setVelocity(float deltaTime) = 0;
        void setVelocity(QVector3D vel);
        void setYPos(float posY);
        void setTriangleNormal(QVector3D normal);

        QVector3D getVelocity() {return mVelocity;}
        QVector3D getTriangleNormal() {return mTriangleNormal;}

        bool mGrounded = false;
        int mCurrentTriangle = -1;

    protected:
        QVector3D mVelocity{0.f, 0.f, 0.f};
        QVector3D mAcceleration{0.f, 0.f, 0.f};
        QVector3D mForce{0.f, 0.f, 0.f};
        QVector3D mTriangleNormal{0.f, 0.f, 0.f};
        float mFriction;
        float mSpeed;
        float mMass{1.f};       
};

#endif // PHYSICSOBJECT_H
