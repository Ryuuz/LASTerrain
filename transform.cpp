#include "transform.h"


Transform::Transform()
{
    mScale = QVector3D(1.f, 1.f, 1.f);
}


Transform::~Transform()
{

}


void Transform::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
    mPosition = QVector3D(x, y, z);
}


void Transform::setRotation(GLfloat x, GLfloat y, GLfloat z)
{
    mRotation = QVector3D(x, y, z);
}


void Transform::setScale(GLfloat x, GLfloat y, GLfloat z)
{
    mScale = QVector3D(x, y, z);
}


void Transform::translate(GLfloat x, GLfloat y, GLfloat z)
{
    mPosition = mPosition + QVector3D(x, y, z);
}


void Transform::rotate(GLfloat x, GLfloat y, GLfloat z)
{
    mRotation = mRotation + QVector3D(x, y, z);
}


void Transform::scale(GLfloat x, GLfloat y, GLfloat z)
{
    mScale = mScale + QVector3D(x, y, z);
}
