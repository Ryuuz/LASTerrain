#include "npc.h"
#include "state.h"
#include "patrolstate.h"
#include "thinkstate.h"
#include "bspline.h"
#include "transform.h"
#include <QDebug>


NPC::NPC(SceneObject *a, BSpline *path) : PhysicsObject (a)
{
    mCurrentState = new ThinkState(path);
}


NPC::~NPC()
{
    delete mCurrentState;
}


void NPC::physicsUpdate(float deltaTime)
{
    mCurrentState->update(*this);
    setVelocity(deltaTime);
    moveObject(deltaTime);
}


void NPC::moveObject(float deltaTime)
{
    QVector3D temp = getPosition();

    QVector3D position = (mVelocity * deltaTime);
    mTransform->translate(position.x(), position.y(), position.z());

    mProgress += (getPosition() - temp).length() * deltaTime;
}


void NPC::setVelocity(float deltaTime)
{
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
        //Acceleration just from force
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
