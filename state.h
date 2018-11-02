#ifndef STATE_H
#define STATE_H

class BSpline;
class NPC;

class State
{
    public:
        State(BSpline *path);
        virtual ~State();

//        virtual void enter() = 0;
        virtual void update(NPC &enemy) = 0;
        virtual State* changeState() = 0;
//        virtual void exit() = 0;

        void setPath(BSpline *path) {mPath = path;}
        BSpline* getPath() {return mPath;}

    protected:
        BSpline *mPath = nullptr;
};

#endif // STATE_H
