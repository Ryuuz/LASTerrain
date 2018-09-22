#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

class Light
{
    public:
        Light(QVector3D pos, QVector3D color = QVector3D(1.f, 1.f, 1.f));
        ~Light();
        void setPosition(QVector3D pos);
        void setLightColor(QVector3D color);

        QVector3D getPosition() {return mPosition;}
        QVector3D getLightColor() {return mLightColor;}

    private:
        QVector3D mPosition;
        QVector3D mLightColor;
};

#endif // LIGHT_H
