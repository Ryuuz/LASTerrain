#ifndef COLORMATERIAL_H
#define COLORMATERIAL_H

#include "material.h"
#include <QtGui/qopengl.h>
#include <QMatrix4x4>
#include <QVector3D>

class Shader;

class ColorMaterial : public Material
{
    public:
        ColorMaterial(Shader *s);
        ~ColorMaterial() override;
        void createMaterial() override;
        void setUniforms(QMatrix4x4 *modelMatrix, QVector3D color = QVector3D(1.f, 1.f, 1.f)) override;

    private:
        GLuint mMatrixUniform;
};

#endif // COLORMATERIAL_H
