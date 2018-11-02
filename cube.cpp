#include "cube.h"
#include "vertex.h"
#include <QVector3D>


Cube::Cube()
{
    mNumberOfVertices = 8;
    mNumberOfIndices = 36;

    //The vertices used for drawing the object
    mVertices = new Vertex[mNumberOfVertices]
    {
        {QVector3D{-0.5f, 0.5f, -0.5f},     QVector3D{0.f, 0.f, -1.f},    QVector2D{0.f, 1.f}},
        {QVector3D{0.5f, 0.5f, -0.5f},      QVector3D{0.f, 0.f, -1.f},    QVector2D{0.f, 0.f}},
        {QVector3D{0.5f, -0.5f, -0.5f},     QVector3D{0.f, 0.f, -1.f},    QVector2D{1.f, 0.f}},
        {QVector3D{-0.5f, -0.5f, -0.5f},    QVector3D{0.f, 0.f, -1.f},    QVector2D{1.f, 0.f}},

        {QVector3D{0.5f, 0.5f, 0.5f},       QVector3D{0.f, 0.f, 1.f},    QVector2D{0.f, 0.f}},
        {QVector3D{0.5f, -0.5f, 0.5f},      QVector3D{0.f, 0.f, 1.f},    QVector2D{1.f, 0.f}},
        {QVector3D{-0.5f, -0.5f, 0.5f},     QVector3D{0.f, 0.f, 1.f},    QVector2D{1.f, 0.f}},
        {QVector3D{-0.5f, 0.5f, 0.5f},      QVector3D{0.f, 0.f, 1.f},    QVector2D{1.f, 1.f}},
    };

    mIndices = new int[mNumberOfIndices]
    {
        //Back
        0, 1, 2,
        2, 3, 0,

        //Right side
        1, 4, 5,
        5, 2, 1,

        //Left side
        0, 3, 6,
        0, 6, 7,

        //Bottom
        6, 3, 2,
        6, 2, 5,

        //Top
        0, 7, 4,
        0, 4, 1,

        //Front
        7, 6, 5,
        7, 5, 4
    };

    findMaxVertex();
    findMinVertex();

    init();
}


Cube::~Cube()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
}


//Initiate everything needed for rendering
void Cube::init()
{
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Sets up and fills the VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mNumberOfVertices*sizeof(Vertex), mVertices, GL_STATIC_DRAW);

    //Element/indices buffer
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumberOfIndices*sizeof(int), mIndices, GL_STATIC_DRAW);

    mDrawType = GL_TRIANGLES;

    delete[] mVertices;
    delete[] mIndices;
}
