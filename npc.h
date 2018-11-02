#ifndef NPC_H
#define NPC_H

#include "physicsobject.h"
#include <QVector3D>

class PointCloud;
class State;
class BSpline;

class NPC : public PhysicsObject
{
    public:
        NPC(SceneObject *a, BSpline *path);
        ~NPC() override;

        void physicsUpdate(float deltaTime) override;
        void moveObject(float deltaTime) override;
        void setVelocity(float deltaTime) override;
        void setState(State *newState) {mCurrentState = newState;}
        void setProgress(float t) {mProgress = t;}
        float getProgress() const {return mProgress;}

    private:
        State *mCurrentState;
        float mProgress = 0.f;
};

#endif // NPC_H
