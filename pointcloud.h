#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include "sceneobject.h"
#include "lasdefinitions.h"
#include <vector>
#include <string>
#include <fstream>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

struct Triangle
{
    QVector3D a, b, c; //Vertices
    GLint n1{-1}, n2{-1}, n3{-1}; //Neighbors
    QVector3D norm; //Triangle normal
};

class PointCloud : public SceneObject
{
public:
    PointCloud(std::string path, float resX = 1.5f, float resZ = 1.5f);
    ~PointCloud() override;

    void readLASHeader(std::string path);
    void readLASPoints(std::ifstream& file);

    void makeGrid(unsigned int minX, unsigned int minZ);
    void assignIndices();
    void averageNormals();

    QVector3D findY(const QVector3D &point, QMatrix4x4 *transformations = nullptr, int tri = -1);
    int findPoint(const QVector3D &point, QMatrix4x4 *transformations = nullptr);
    const QVector3D barycentricCoordinates(const QVector3D &p, const QVector3D &q, const QVector3D &r, const QVector3D &point);

    Triangle getTriangle(int i);

private:
    void init() override;
    std::vector<QVector3D> mPoints;
    Triangle *mTriangles;
    int mNumberOfTriangles;
    float mResolutionX;
    float mResolutionZ;
    int stepX = 0;
    int stepZ = 0;

    char ushortbuffer[2];
    char ulongbuffer[4];
    char ulonglongbuffer[8];

    PUBLIC_HEADER_BLOCK mHeader;
};

#endif // POINTCLOUD_H
