#ifndef THINKSTATE_H
#define THINKSTATE_H

#include "state.h"

class BSpline;
class NPC;
class PatrolState;

class ThinkState : public State
{
public:
    ThinkState(BSpline *path, PatrolState *state = nullptr);
    ~ThinkState() override;

    void update(NPC &enemy) override;
    State* changeState() override;

private:
    PatrolState *mPatrol;
};

#endif // THINKSTATE_H
