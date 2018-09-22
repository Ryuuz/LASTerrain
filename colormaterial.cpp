#include "colormaterial.h"
#include "shader.h"
#include "camera.h"


ColorMaterial::ColorMaterial(Shader *s) : Material(s)
{

}


ColorMaterial::~ColorMaterial()
{

}


void ColorMaterial::createMaterial()
{
    initializeOpenGLFunctions();

    Material::createMaterial();

    mMatrixUniform = glGetUniformLocation(mShader->getProgram(), "matrix");
}


void ColorMaterial::setUniforms(QMatrix4x4 *modelMatrix, QVector3D color)
{
    QMatrix4x4 mvp = (*mCamera->getPerspectiveMatrix())*(*mCamera->getViewMatrix())*(*modelMatrix);

    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mvp.constData());
}
