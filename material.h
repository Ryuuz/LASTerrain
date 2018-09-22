#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <QVector3D>
#include <QtGui/qopengl.h>

class Shader;
class Camera;

class Material : protected QOpenGLFunctions_4_1_Core
{
    public:
        Material(Shader* s);
        ~Material();
        virtual void createMaterial();
        virtual void setUniforms(QMatrix4x4 *modelMatrix, QVector3D color = QVector3D(1.f, 1.f, 1.f)) = 0;
        static void setCamera(Camera *camera);

        Shader* getShader() const {return mShader;}

    protected:
        Shader *mShader;
        static Camera *mCamera;
};

#endif // MATERIAL_H
