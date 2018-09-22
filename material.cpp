#include "material.h"
#include "shader.h"
#include "vertex.h"
#include "camera.h"

Camera* Material::mCamera;

Material::Material(Shader *s) : mShader(s)
{

}


Material::~Material()
{

}


void Material::createMaterial()
{
    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
}


void Material::setCamera(Camera *camera)
{
    mCamera = camera;
}
