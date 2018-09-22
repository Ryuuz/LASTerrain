#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include <QVector3D>

class ObjectInstance;

enum BoundType
{
    sphere,
    box,
    trigger,
    plane
};

class BoundingVolume
{
    public:
        BoundingVolume(QVector3D a, QVector3D b, BoundType t, ObjectInstance *o);
        ~BoundingVolume();

        QVector3D getMax();
        QVector3D getMin();
        const BoundType& getType() const {return mType;}

    protected:        
        QVector3D mMax;
        QVector3D mMin;
        BoundType mType;
        ObjectInstance *mOwner;
};

#endif // BOUNDINGVOLUME_H
