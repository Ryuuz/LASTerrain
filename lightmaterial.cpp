#include "lightmaterial.h"
#include "shader.h"
#include "light.h"
#include "camera.h"


LightMaterial::LightMaterial(Shader *s, Light *l) : Material(s), mLight(l)
{
    mAmbientIntensity = 0.2;
}


LightMaterial::~LightMaterial()
{
    mShader = nullptr;
    mLight = nullptr;
}


void LightMaterial::createMaterial()
{
    initializeOpenGLFunctions();

    Material::createMaterial();

    mMatrixUniform = glGetUniformLocation(mShader->getProgram(), "matrix");
    mMVMatrixUniform = glGetUniformLocation(mShader->getProgram(), "mvMatrix");
    mNormalUniform = glGetUniformLocation(mShader->getProgram(), "nMatrix");

    mLightColor = glGetUniformLocation(mShader->getProgram(), "lightColor");
    mObjectColor = glGetUniformLocation(mShader->getProgram(), "objectColor");
    mLightPos = glGetUniformLocation(mShader->getProgram(), "lightPos");
}


void LightMaterial::setUniforms(QMatrix4x4 *modelMatrix, QVector3D color)
{
    QMatrix4x4 mv = (*mCamera->getViewMatrix())*(*modelMatrix);
    QMatrix4x4 mvp = (*mCamera->getPerspectiveMatrix())*(*mCamera->getViewMatrix())*(*modelMatrix);
    QMatrix3x3 normalMatrix = mv.normalMatrix();

    glUniformMatrix4fv(mMVMatrixUniform, 1, GL_FALSE, mv.constData());
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mvp.constData());
    glUniformMatrix3fv(mNormalUniform, 1, GL_FALSE, normalMatrix.constData());

    glUniform3f(mLightColor, mLight->getLightColor().x(), mLight->getLightColor().y(), mLight->getLightColor().z());
    glUniform3f(mObjectColor, color.x(), color.y(), color.z());
    glUniform3f(mLightPos, mLight->getPosition().x(), mLight->getPosition().y(), mLight->getPosition().z());
}
