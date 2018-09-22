#include "camera.h"
#include "transform.h"
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector4D>
#include <QQuaternion>


Camera::Camera()
{
    mViewMatrix = new QMatrix4x4;
    mPerspectiveMatrix = new QMatrix4x4;
    mTransform = new Transform;

    mViewMatrix->setToIdentity();
    mPerspectiveMatrix->setToIdentity();
}


Camera::~Camera()
{
    delete mViewMatrix;
    delete mPerspectiveMatrix;
}


void Camera::setViewMatrix()
{
    mViewMatrix->setToIdentity();

    mDirection = (getCameraPosition() - mTarget).normalized();

    mRight = QVector3D::crossProduct(QVector3D(0.f, 1.f, 0.f), mDirection);
    mRight.normalize();

    mUp = QVector3D::crossProduct(mDirection, mRight);
    mUp.normalize();

    mViewMatrix->lookAt(getCameraPosition(), mTarget, QVector3D(0.f, 1.f, 0.f));
}


void Camera::setPerspectiveMatrix(int width, int height, float fieldOfView, float nearPlane, float farPlane)
{
    float aspectRatio = static_cast<float>(width)/static_cast<float>(height ? height : 1);
    mPerspectiveMatrix->perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
}


void Camera::setTarget(QVector3D target)
{
    mTarget = target;
    setViewMatrix();
}


QVector3D Camera::getCameraPosition()
{
    QVector4D position(0.f, 0.f, 0.f, 1.f);
    QMatrix4x4 temp;

    temp.setToIdentity();

    temp.rotate(QQuaternion::fromEulerAngles(mTransform->getRotation()));
    temp.translate(mTransform->getTranslation());

    position = temp*position;

    return position.toVector3D();
}
