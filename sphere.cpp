#include "sphere.h"
#include "vertex.h"
#include <QVector3D>
#include <cmath>


Sphere::Sphere(int n) : mRecursions(n), mIndex(0)
{
    mNumberOfVertices = 3*8*std::pow(4, mRecursions);
    mVertices = new Vertex[mNumberOfVertices];
    makeSphere();

    findMaxVertex();
    findMinVertex();

    init();
}


Sphere::~Sphere()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}


//Initiate everything needed for rendering
void Sphere::init()
{
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Sets up and fills the VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mNumberOfVertices*sizeof(Vertex), mVertices, GL_STATIC_DRAW);

    mDrawType = GL_TRIANGLES;

    delete[] mVertices;
}


//Makes a sphere by subdividing each of the sides of an octrahedon
void Sphere::makeSphere()
{
    //The vertices for the octrahedon
    QVector3D v0(1.f, 0.f, 0.f);
    QVector3D v1(0.f, 1.f, 0.f);
    QVector3D v2(0.f, 0.f, 1.f);

    QVector3D v3(-1.f, 0.f, 0.f);
    QVector3D v4(0.f, -1.f, 0.f);
    QVector3D v5(0.f, 0.f, -1.f);

    subdivide(v0, v1, v2, mRecursions);
    subdivide(v2, v1, v3, mRecursions);
    subdivide(v3, v1, v5, mRecursions);
    subdivide(v5, v1, v0, mRecursions);

    subdivide(v2, v4, v0, mRecursions);
    subdivide(v0, v4, v5, mRecursions);
    subdivide(v5, v4, v3, mRecursions);
    subdivide(v3, v4, v2, mRecursions);
}


//Subdivides the triangle into four triangles
void Sphere::subdivide(const QVector3D &u, const QVector3D &v, const QVector3D &w, int n)
{
    if(n > 0)
    {
        QVector3D v1 = (u+v).normalized();
        QVector3D v2 = (u+w).normalized();
        QVector3D v3 = (w+v).normalized();

        subdivide(u, v1, v2, n-1);
        subdivide(v, v3, v1, n-1);
        subdivide(w, v2, v3, n-1);
        subdivide(v3, v2, v1, n-1);
    }
    else
    {
        makeTriangle(u, v, w);
    }
}


//Adds the vertices of the triangle to the vertex array
void Sphere::makeTriangle(const QVector3D &u, const QVector3D &v, const QVector3D &w)
{
    QVector3D norm = QVector3D::crossProduct((v - u), (w - u));
    norm.normalize();

    mVertices[mIndex].setPosition(u);
    mVertices[mIndex].setNormal(norm);
    mVertices[mIndex].setUV(0.f, 0.f);
    mIndex++;

    mVertices[mIndex].setPosition(v);
    mVertices[mIndex].setNormal(norm);
    mVertices[mIndex].setUV(0.f, 0.f);
    mIndex++;

    mVertices[mIndex].setPosition(w);
    mVertices[mIndex].setNormal(norm);
    mVertices[mIndex].setUV(0.f, 0.f);
    mIndex++;
}
