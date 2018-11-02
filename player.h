#ifndef PLAYER_H
#define PLAYER_H

#include "physicsobject.h"
#include <QVector3D>

class QKeyEvent;
class PointCloud;

class Player : public PhysicsObject
{
    public:
        Player(SceneObject *a);
        ~Player() override;

        void physicsUpdate(float deltaTime) override;
        void moveObject(float deltaTime) override;
        void handleInput(QKeyEvent *event, bool pressed);
        void setVelocity(float deltaTime) override;

    private:
        bool mWPressed = false;
        bool mAPressed = false;
        bool mSPressed = false;
        bool mDPressed = false;
        bool mSpacePressed = false;
};

#endif // PLAYER_H
