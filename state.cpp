#include "state.h"
#include "bspline.h"
#include "patrolstate.h"
#include "thinkstate.h"


State::State(BSpline *path) : mPath(path)
{

}


State::~State()
{
    mPath = nullptr;
}
