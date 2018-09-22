#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QVector3D>

class Vertex;
class Material;

class SceneObject : protected QOpenGLFunctions_4_1_Core
{
    public:
        SceneObject();
        ~SceneObject();       

        void setMaterial(Material *mat);

        int getVertexNumber() const {return mNumberOfVertices;}
        int getIndexNumber() const {return mNumberOfIndices;}
        GLuint getVao() const {return mVAO;}
        QVector3D getMaxVertex() const {return mMaxVertex;}
        QVector3D getMinVertex() const {return mMinVertex;}
        Material* getMaterial() const {return mMaterial;}

        GLenum mDrawType;

    protected:
        void findMaxVertex();
        void findMinVertex();
        virtual void init() = 0;

        Vertex *mVertices;
        int *mIndices;
        int mNumberOfVertices;
        int mNumberOfIndices;
        GLuint mVBO;
        GLuint mVAO;
        GLuint mEBO;
        QVector3D mMaxVertex;
        QVector3D mMinVertex;
        Material *mMaterial;
};

#endif // SCENEOBJECT_H
