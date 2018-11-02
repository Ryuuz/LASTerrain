#include "patrolstate.h"
#include "bspline.h"
#include "npc.h"
#include "thinkstate.h"
#include "transform.h"
#include <QVector3D>
#include <QDebug>


PatrolState::PatrolState(BSpline *path, ThinkState *state) : State(path), mThinking(state)
{

}


PatrolState::~PatrolState()
{

}


void PatrolState::update(NPC &enemy)
{
    if((enemy.getProgress() + mStep) > 1.f)
    {
        enemy.setState(changeState());
    }
    else
    {
        QVector3D direction = mPath->findPoint(enemy.getProgress() + mStep);
        direction = direction - enemy.getPosition();
        direction.setY(0.f);

        if(direction.x() > 0.f)
        {
            direction.setX(1.f);
        }
        else if(direction.x() < 0.f)
        {
            direction.setX(-1.f);
        }

        if(direction.z() > 0.f)
        {
            direction.setZ(1.f);
        }
        else if(direction.z() < 0.f)
        {
            direction.setZ(-1.f);
        }

        enemy.addForce(direction * (enemy.mGrounded ? 10.f : 2.f));
    }
}


State* PatrolState::changeState()
{
    if(!mThinking)
    {
        mThinking = new ThinkState(mPath, this);
    }

    return mThinking;
}
