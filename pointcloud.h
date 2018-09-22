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

class PointCloud : public SceneObject
{
public:
    PointCloud(std::string path, float resX = 1.5f, float resZ = 1.5f);
    ~PointCloud() override;

    void readLASHeader(std::string path);
    void readLASPoints(std::ifstream& file);

    void makeGrid(unsigned int minX, unsigned int minZ);
    void assignIndices();
    void assignNeighbors();
    void averageNormals();

    float findY(const QVector3D &point, QMatrix4x4 *transformations);
    int findPoint(const QVector3D &point);
    const QVector3D barycentricCoordinates(const QVector3D &p, const QVector3D &q, const QVector3D &r, const QVector3D &point);

private:
    void init() override;
    std::vector<QVector3D> mPoints;
    float mResolutionX;
    float mResolutionZ;
    int *mNeighbors = nullptr;
    int stepX = 0;
    int stepZ = 0;

    char ushortbuffer[2];
    char ulongbuffer[4];
    char ulonglongbuffer[8];

    PUBLIC_HEADER_BLOCK mHeader;
};

#endif // POINTCLOUD_H
