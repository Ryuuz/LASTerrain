#include "light.h"


Light::Light(QVector3D pos, QVector3D color) : mPosition(pos), mLightColor(color)
{

}


Light::~Light()
{

}


void Light::setPosition(QVector3D pos)
{
    mPosition = pos;
}


void Light::setLightColor(QVector3D color)
{
    mLightColor = color;
}
