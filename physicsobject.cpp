#include "physicsobject.h"
#include "transform.h"
#include "sceneobject.h"
#include "pointcloud.h"
#include <QMatrix4x4>
#include <QKeyEvent>


PhysicsObject::PhysicsObject(SceneObject *a) : ObjectInstance(a)
{
    mVelocity = QVector3D(0.f, 0.f, 0.f);
    mFriction = 0.74f;
    mSpeed = 10.f;
}


PhysicsObject::~PhysicsObject()
{

}


//Moves the object based on the velocity after taking friction into consideration
void PhysicsObject::physicsUpdate(float deltaTime)
{
    setVelocity(deltaTime);

    //Not a very good way to add friction, but it will do for this program
    mVelocity = mVelocity*mFriction;

    if(mVelocity.length() < 0.004f)
    {
        mVelocity = QVector3D(0.f, 0.f, 0.f);
    }
}


void PhysicsObject::moveObject()
{
    mTransform->translate(mVelocity.x(), mVelocity.y(), mVelocity.z());
}


void PhysicsObject::handleInput(QKeyEvent *event, bool pressed)
{
    switch (event->key())
    {
        case Qt::Key_W : mWPressed = pressed;
            break;

        case Qt::Key_A : mAPressed = pressed;
            break;

        case Qt::Key_S : mSPressed = pressed;
            break;

        case Qt::Key_D :mDPressed = pressed;
            break;

        default:
            break;
    }
}


//Sets velocity based on the pressed key
void PhysicsObject::setVelocity(float deltaTime)
{
    if(mWPressed)
    {
        mVelocity.setZ(-1.f);
    }
    else if(mSPressed)
    {
        mVelocity.setZ(1.f);
    }

    if(mAPressed)
    {
        mVelocity.setX(-1.f);
    }
    else if(mDPressed)
    {
        mVelocity.setX(1.f);
    }

    mVelocity *= mSpeed*deltaTime;

    //Makes sure it doesn't go faster than 'mSpeed'
    if(mVelocity.length() > mSpeed)
    {
        mVelocity.normalize();
    }
}


//Sets the velocity to be 'vel' and clamps the speed
void PhysicsObject::setVelocity(QVector3D vel)
{
    mVelocity = vel;
    mVelocity *= mSpeed;

    if(mVelocity.length() > mSpeed)
    {
        mVelocity.normalize();
    }
}


//Sets the y position of the object
void PhysicsObject::setYPos(float posY)
{
    float radius = ((mBoundingObject->getMax() - mBoundingObject->getMin()).length()/2);

    mTransform->setPosition(mTransform->getTranslation().x(), (posY + radius), mTransform->getTranslation().z());
}
