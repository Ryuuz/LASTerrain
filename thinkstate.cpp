#include "thinkstate.h"
#include "bspline.h"
#include "npc.h"
#include "patrolstate.h"
#include <QDebug>


ThinkState::ThinkState(BSpline *path, PatrolState *state) : State(path), mPatrol(state)
{

}


ThinkState::~ThinkState()
{

}


void ThinkState::update(NPC &enemy)
{
    mPath->reorderPoints(enemy.getPosition());
    mPath->makeKnots();

    enemy.setProgress(0.f);
    enemy.setState(changeState());
}


State* ThinkState::changeState()
{
    if(!mPatrol)
    {
        mPatrol = new PatrolState(mPath, this);
    }

    return mPatrol;
}
