#ifndef CAMERA_H
#define CAMERA_H

#include <QtGui/qopengl.h>
#include <QVector3D>

class QMatrix4x4;
class Transform;

class Camera
{
    public:
        Camera();
        ~Camera();

        void setViewMatrix();
        void setPerspectiveMatrix(int width, int height, float fieldOfView = 45.f, float nearPlane = 0.1f, float farPlane = 1000.f);
        void setTarget(QVector3D target);

        const QMatrix4x4* getViewMatrix() const {return mViewMatrix;}
        const QMatrix4x4* getPerspectiveMatrix() const {return mPerspectiveMatrix;}
        Transform* getTransform() const {return mTransform;}
        QVector3D getCameraPosition();
        QVector3D getRight() const {return mRight;}
        QVector3D getUp() const {return mUp;}

    private:
        Transform *mTransform;
        QMatrix4x4 *mViewMatrix;
        QMatrix4x4 *mPerspectiveMatrix;
        QVector3D mTarget;
        QVector3D mUp;
        QVector3D mRight;
        QVector3D mDirection;
};

#endif // CAMERA_H
