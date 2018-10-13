#ifndef COLLISION_H
#define COLLISION_H

#include <QObject>

class BoundingVolume;
class ObjectInstance;
class PhysicsObject;

class Collision : public QObject
{
    Q_OBJECT

    public:
        Collision();
        ~Collision();

        bool collisionDetection(BoundingVolume *obj1, BoundingVolume *obj2);
        void collisionHandling(PhysicsObject *obj1, ObjectInstance *obj2);
        void collisionHandling(PhysicsObject *obj1, PhysicsObject *obj2);
        void groundCollision(PhysicsObject *obj1, ObjectInstance *ground);

    signals:
        void restart();

    private:

};

#endif // COLLISION_H
