#ifndef PATROLSTATE_H
#define PATROLSTATE_H

#include "state.h"

class BSpline;
class NPC;
class ThinkState;

class PatrolState : public State
{
public:
    PatrolState(BSpline *path, ThinkState *state = nullptr);
    ~PatrolState() override;

    void update(NPC &enemy) override;
    State* changeState() override;

private:
    ThinkState *mThinking;
    float mStep = 0.02f;
};

#endif // PATROLSTATE_H
