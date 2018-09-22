#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QVector3D>
#include <QtGui/qopengl.h>

class Transform
{
    public:
        Transform();
        ~Transform();

        void setPosition(GLfloat x, GLfloat y, GLfloat z);
        void setRotation(GLfloat x, GLfloat y, GLfloat z);
        void setScale(GLfloat x, GLfloat y, GLfloat z);

        void translate(GLfloat x, GLfloat y, GLfloat z);
        void rotate(GLfloat x, GLfloat y, GLfloat z);
        void scale(GLfloat x, GLfloat y, GLfloat z);

        QVector3D getTranslation() const {return mPosition;}
        QVector3D getRotation() const {return mRotation;}
        QVector3D getScale() const {return mScale;}


    private:
        QVector3D mPosition;
        QVector3D mRotation;
        QVector3D mScale;
};

#endif // TRANSFORM_H
