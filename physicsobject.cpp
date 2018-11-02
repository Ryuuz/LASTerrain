#include "physicsobject.h"
#include "transform.h"
#include "sceneobject.h"
#include "pointcloud.h"
#include "constants.h"
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QDebug>


PhysicsObject::PhysicsObject(SceneObject *a) : ObjectInstance(a)
{
    mFriction = 0.98f;
    mSpeed = 30.f;
}


PhysicsObject::~PhysicsObject()
{

}


//Moves the object based on the velocity after taking friction into consideration
void PhysicsObject::physicsUpdate(float deltaTime)
{
    moveObject(deltaTime);
}


void PhysicsObject::moveObject(float deltaTime)
{
    QVector3D position = (mVelocity * deltaTime);
    mTransform->translate(position.x(), position.y(), position.z());
}


void PhysicsObject::addForce(QVector3D force)
{
    mForce = mForce + force;
}


//Sets the velocity to be 'vel' and clamps the speed
void PhysicsObject::setVelocity(QVector3D vel)
{
    mVelocity = vel;

    if(mVelocity.length() > mSpeed)
    {
        mVelocity.normalize();
        mVelocity *= mSpeed;
    }
}


//Sets the y position of the object
void PhysicsObject::setYPos(float posY)
{
    mTransform->setPosition(mTransform->getTranslation().x(), posY, mTransform->getTranslation().z());
}


void PhysicsObject::setTriangleNormal(QVector3D normal)
{
    mTriangleNormal = normal;
}
