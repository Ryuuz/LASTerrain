#include "objectinstance.h"
#include "sceneobject.h"
#include "camera.h"
#include "shader.h"
#include "transform.h"
#include "material.h"
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector4D>
#include <QQuaternion>


ObjectInstance::ObjectInstance(SceneObject *a) : mModel(a)
{
    mModelMatrix = new QMatrix4x4;
    mModelMatrix->setToIdentity();
    mBoundingObject = nullptr;
    mObjectColor = QVector3D(1.f, 1.f, 1.f);
    mTransform = new Transform;
}


ObjectInstance::~ObjectInstance()
{
    delete mModelMatrix;
    delete mBoundingObject;
    delete mTransform;
    mModel = nullptr;
}


//Calculates the MVP matrix and draws the object using the given 'mDrawType'
void ObjectInstance::draw()
{
    if(mModel && !mDestroyed)
    {
        initializeOpenGLFunctions();

        glUseProgram(mModel->getMaterial()->getShader()->getProgram());
        glBindVertexArray(mModel->getVao());

        mModel->getMaterial()->setUniforms(getModelMatrix(), mObjectColor);


        if(!mModel->getIndexNumber())
        {
            glDrawArrays(mModel->mDrawType, 0, mModel->getVertexNumber());
        }
        else
        {
            glDrawElements(mModel->mDrawType, mModel->getIndexNumber()*sizeof(int), GL_UNSIGNED_INT, (void*)0);
        }

        glUseProgram(0);
    }
}


//Creates bounding volume of given type based on the model
void ObjectInstance::setBoundingObject(BoundType type)
{
    if(mModel)
    {
        mBoundingObject = new BoundingVolume(mModel->getMaxVertex(), mModel->getMinVertex(), type, this);
    }
}


//Creates a bounding volume with the given values
void ObjectInstance::setBoundingObject(QVector3D max, QVector3D min, BoundType type)
{
    mBoundingObject = new BoundingVolume(max, min, type, this);
}


//Performs all transformations on the model matrix and returns it
QMatrix4x4* ObjectInstance::getModelMatrix()
{
    mModelMatrix->setToIdentity();
    mModelMatrix->translate(mTransform->getTranslation());
    mModelMatrix->rotate(QQuaternion::fromEulerAngles(mTransform->getRotation()));
    mModelMatrix->scale(mTransform->getScale());

    return mModelMatrix;
}


QVector3D ObjectInstance::getPosition()
{
    QVector3D position = mTransform->getTranslation();
    position = ((*getModelMatrix()) * QVector4D(position, 1.f)).toVector3D();

    return position;
}
