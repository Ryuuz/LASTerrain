#ifndef LIGHTMATERIAL_H
#define LIGHTMATERIAL_H

#include "material.h"
#include <QtGui/qopengl.h>
#include <QMatrix4x4>
#include <QVector3D>

class Shader;
class Light;

class LightMaterial : public Material
{
    public:
        LightMaterial(Shader *s, Light *l);
        ~LightMaterial();
        void createMaterial() override;
        void setUniforms(QMatrix4x4 *modelMatrix, QVector3D color = QVector3D(1.f, 1.f, 1.f)) override;

    private:
        Light *mLight;
        float mAmbientIntensity;

        GLuint mMatrixUniform;
        GLuint mMVMatrixUniform;
        GLuint mNormalUniform;

        GLuint mLightColor;
        GLuint mObjectColor;
        GLuint mLightPos;
};

#endif // LIGHTMATERIAL_H
