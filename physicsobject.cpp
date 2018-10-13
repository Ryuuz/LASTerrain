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
    setVelocity(deltaTime);
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

        case Qt::Key_D : mDPressed = pressed;
            break;

        case Qt::Key_Space : mSpacePressed = pressed;
            break;

        default:
            break;
    }
}


//Sets velocity based on the pressed key
void PhysicsObject::setVelocity(float deltaTime)
{
    if(mTag == gsl::player)
    {
        QVector3D input(0.f, 0.f, 0.f);

        if(mWPressed)
        {
            input.setZ(-1.f);
        }
        else if(mSPressed)
        {
            input.setZ(1.f);
        }

        if(mAPressed)
        {
            input.setX(-1.f);
        }
        else if(mDPressed)
        {
            input.setX(1.f);
        }

        if(mSpacePressed && mGrounded)
        {
            input.setY(50.f);
        }

        mForce = mForce + (input * (mGrounded ? 10.f : 2.f));
    }

    if(mGrounded)
    {
        mAcceleration = mTriangleNormal;
        mAcceleration.setX(mAcceleration.x()*mAcceleration.y());
        mAcceleration.setZ(mAcceleration.z()*mAcceleration.y());
        mAcceleration.setY((mAcceleration.y()*mAcceleration.y())-1);

        mAcceleration = (mAcceleration * 9.81f) + mForce;
        mAcceleration = mAcceleration * (1.f/mMass);
    }
    else
    {
        //acceleration just from force
        mAcceleration = (mForce + QVector3D{0.f, -9.81f, 0.f}) * (1.f/mMass);
    }

    mVelocity = mVelocity + (mAcceleration * deltaTime);

    if(mVelocity.length() > mSpeed)
    {
        mVelocity.normalize();
        mVelocity = mVelocity * mSpeed;
    }

    //Add friction
    mVelocity = mVelocity * mFriction;

    //Set velocity to 0 if below a certain magnitude
    if(mGrounded && mVelocity.length() < 0.08f)
    {
        mVelocity = QVector3D{0.f, 0.f, 0.f};
    }

    //reset forces
    mForce = QVector3D(0.f, 0.f, 0.f);
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
