#include "pointcloud.h"
#include "vertex.h"
#include "constants.h"
#include "byteconverter.h"


PointCloud::PointCloud(std::string path, float resX, float resZ) : mResolutionX(resX), mResolutionZ(resZ)
{
    readLASHeader(path);
    assignIndices();
    averageNormals();
    assignNeighbors();
    init();
}


PointCloud::~PointCloud()
{

}


void PointCloud::readLASHeader(std::string path)
{
    std::ifstream file(gsl::assetFilePath + path, std::ios::in | std::ios::binary);

    if(!file.is_open())
    {
        return;
    }

    char versionMinor;

    //Read minor version number
    file.seekg(25);
    file.read(&versionMinor, 1);
    mHeader.versionMinor = static_cast<unsigned char>(versionMinor);

    //Read offset to point data
    file.seekg(70, std::ios::cur);
    file.read(&ulongbuffer[0], 4);
    ByteConverter::bytesToUnsignedLong(ulongbuffer, &mHeader.offsetToPointData);

    //Read point data record length
    file.seekg(5, std::ios::cur);
    file.read(&ushortbuffer[0], 2);
    ByteConverter::bytesToUnsignedShort(ushortbuffer, &mHeader.pointDataRecordLength);

    //Read legacy number of point records
    file.read(&ulongbuffer[0], 4);
    ByteConverter::bytesToUnsignedLong(ulongbuffer, &mHeader.legacyNumberOfPointRecords);

    //Read x, y, and z scale factor
    file.seekg(20, std::ios::cur);
    file.read(&ulonglongbuffer[0], 8);
    ByteConverter::bytesToDouble(ulonglongbuffer, &mHeader.xScaleFactor);
    file.read(&ulonglongbuffer[0], 8);
    ByteConverter::bytesToDouble(ulonglongbuffer, &mHeader.yScaleFactor);
    file.read(&ulonglongbuffer[0], 8);
    ByteConverter::bytesToDouble(ulonglongbuffer, &mHeader.zScaleFactor);

    //Read x, y, and z offset
    file.read(&ulonglongbuffer[0], 8);
    ByteConverter::bytesToDouble(ulonglongbuffer, &mHeader.xOffset);
    file.read(&ulonglongbuffer[0], 8);
    ByteConverter::bytesToDouble(ulonglongbuffer, &mHeader.yOffset);
    file.read(&ulonglongbuffer[0], 8);
    ByteConverter::bytesToDouble(ulonglongbuffer, &mHeader.zOffset);

    if(mHeader.versionMinor == 4)
    {
        //Read number of point records
        file.seekg(247);
        file.read(&ulonglongbuffer[0], 8);
        ByteConverter::bytesToUnsignedLongLong(ulonglongbuffer, &mHeader.numberOfPointRecords);
    }

    file.seekg(mHeader.offsetToPointData);
    readLASPoints(file);
}


void PointCloud::readLASPoints(std::ifstream& file)
{
    bool using14 = mHeader.versionMinor == 4;
    long long recordsLeft = static_cast<long long>(using14 ? mHeader.numberOfPointRecords : mHeader.legacyNumberOfPointRecords);
    long long recordsRead = 0;
    int recordsToRead = 10000;
    long temp;
    float x, y, z;
    unsigned int minX = 0, maxX = 0, minZ = 0, maxZ = 0;

    char* data = new char[recordsToRead * mHeader.pointDataRecordLength];
    if(mPoints.size())
    {
        mPoints.clear();
    }

    while(recordsLeft > 0)
    {
        if(recordsLeft - recordsToRead >= 0)
        {
            file.read(data, static_cast<int>(recordsToRead * mHeader.pointDataRecordLength));
            recordsRead = recordsToRead;
        }
        else
        {
            file.read(data, static_cast<int>(recordsLeft * mHeader.pointDataRecordLength));
            recordsRead = recordsLeft;
        }

        recordsLeft -= recordsRead;

        for(long long i = 0; i < recordsRead; i++)
        {
            memcpy(ulongbuffer, &data[i * mHeader.pointDataRecordLength], 4);
            ByteConverter::bytesToLong(ulongbuffer, &temp);
            x = static_cast<float>(temp) * (static_cast<float>(mHeader.xScaleFactor)) + static_cast<float>(mHeader.xOffset);

            memcpy(ulongbuffer, &data[i * mHeader.pointDataRecordLength + 4], 4);
            ByteConverter::bytesToLong(ulongbuffer, &temp);
            z = static_cast<float>(temp) * (static_cast<float>(mHeader.zScaleFactor)) + static_cast<float>(mHeader.zOffset);

            memcpy(ulongbuffer, &data[i * mHeader.pointDataRecordLength + 8], 4);
            ByteConverter::bytesToLong(ulongbuffer, &temp);
            y = static_cast<float>(temp) * (static_cast<float>(mHeader.yScaleFactor)) + static_cast<float>(mHeader.yOffset);

            mPoints.push_back(QVector3D(x, y, z));

            //Min and max x
            if(mPoints.at(minX).x() > x)
            {
                minX = mPoints.size()-1;
            }
            else if(mPoints.at(maxX).x() < x)
            {
                maxX = mPoints.size()-1;
            }

            //Min and max z
            if(mPoints.at(minZ).z() > z)
            {
                minZ = mPoints.size()-1;
            }
            else if(mPoints.at(maxZ).z() < z)
            {
                maxZ = mPoints.size()-1;
            }
        }
    }

    delete[] data;
    file.close();

    stepX = static_cast<int>((mPoints.at(maxX).x() - mPoints.at(minX).x()) / mResolutionX) + 1;
    stepZ = static_cast<int>((mPoints.at(maxZ).z() - mPoints.at(minZ).z()) / mResolutionZ) + 1;
    makeGrid(minX, minZ);
}


void PointCloud::makeGrid(unsigned int minX, unsigned int minZ)
{
    std::vector<QVector3D> *grid = new std::vector<QVector3D>[stepX*stepZ];
    float offsetX = 0.f - mPoints.at(minX).x();
    float offsetZ = 0.f - mPoints.at(minZ).z();

    for(auto point : mPoints)
    {
        grid[((static_cast<int>((point.z() + offsetZ)/mResolutionZ))*(stepX)) + (static_cast<int>((point.x() + offsetX)/mResolutionX))].push_back(point);
    }
    mPoints.clear();

    mNumberOfVertices = static_cast<int>(stepX*stepZ);
    mVertices = new Vertex[mNumberOfVertices];

    float x, y, z;
    int steps[4] = {static_cast<int>(stepX), static_cast<int>(-stepX), 1, -1};

    for(int i = 0; i < mNumberOfVertices; i++)
    {
        y = 0;
        x = (i % stepX) * mResolutionX;
        z = (i / stepX) * mResolutionZ;

        if(grid[i].size() == 0)
        {
            int j;
            int count = 0;
            bool found = false;

            for(int k = 0; k < 4; k++)
            {
                j = i + steps[k];

                while(!found && ((j < mNumberOfVertices) && (j >= 0)))
                {
                    if(grid[j].size() > 0)
                    {
                        y += grid[j].at(0).y();
                        found = true;
                    }

                    j += steps[k];
                }

                if(found)
                {
                    count++;
                    found = false;
                }
            }

            if(count != 0)
            {
                y /= count;
            }
        }
        else if(grid[i].size() == 1)
        {
            y = grid[i].at(0).y();
        }
        else
        {
            for(auto point : grid[i])
            {
                y += point.y();
            }
            y /= static_cast<float>(grid[i].size());           
        }

        mVertices[i].setPosition(x, y, z);
        mVertices[i].setNormal(0.f, 1.f, 0.f);
        mVertices[i].setUV(0.f, 0.f);
    }

    delete[] grid;
}


//Assigns the indices used for drawing
void PointCloud::assignIndices()
{
    mNumberOfIndices = ((stepX-1)*(stepZ-1))*2*3;
    mIndices = new int[mNumberOfIndices];

    int index = 0;

    for(int i = 0; i < (stepZ-1); i++)
    {
        for(int j = 0; j < (stepX-1); j++)
        {
            mIndices[index++] = j + (i*stepX);
            mIndices[index++] = (j + (i*stepX)) + stepX;
            mIndices[index++] = (j + (i*stepX)) + (stepX + 1);

            mIndices[index++] = j + (i*stepX);
            mIndices[index++] = (j + (i*stepX)) + (stepX + 1);
            mIndices[index++] = (j + (i*stepX)) + 1;
        }
    }
}


//Assigns the neighbours to each triangle based on whether they share an edge
void PointCloud::assignNeighbors()
{
    int trianglesX = (stepX - 1) * 2;
    mNeighbors = new int[mNumberOfIndices];

    //Go through each square (every two triangles)
    for(int i = 0; i < mNumberOfIndices; i += 6)
    {
        mNeighbors[i] = (((i/3) + trianglesX) < (trianglesX*(stepZ-1))) ? ((i/3) + trianglesX) : -1;
        mNeighbors[i+1] = (i/3) + 1;
        mNeighbors[i+2] = (((i/3) % trianglesX) == 0) ? -1 : (i/3)-1;

        mNeighbors[i+3] = ((((i/3)+1) % (trianglesX-1)) == 0) ? -1 : ((i/3)+1)+1;
        mNeighbors[i+4] = ((((i/3)+1) - trianglesX) >= 0) ? (((i/3)+1) - trianglesX) : -1;
        mNeighbors[i+5] = ((i/3)+1) - 1;
    }
}


//Calculates the normals by adding the normal of each triangle a vertex is part
void PointCloud::averageNormals()
{
    QVector3D *normals = new QVector3D[mNumberOfIndices/3];
    QVector3D pointNormal;
    int *triangles = new int[6];
    int difference = 0;
    int trianglesX = (stepX-1)*2;

    for(int i = 0; i < mNumberOfIndices; i += 3)
    {
        normals[i/3] = QVector3D::crossProduct((mVertices[mIndices[i+1]].getPosition() - mVertices[mIndices[i]].getPosition()), (mVertices[mIndices[i+2]].getPosition() - mVertices[mIndices[i]].getPosition()));
    }

    for(int i = 0; i < (stepZ); i++)
    {
        for(int j = 0; j < (stepX); j++)
        {
            //If it is the last point in a row (meaning it's at the right edge)
            if((j % (stepX-1)) == 0)
            {
                //Then get the second triangle in the previous square
                triangles[0] = (((j + (i*stepX)) - difference) * 2) - 1;

                //Get the corresponding triangle above (checks for out of bound later)
                triangles[0] = triangles[0] -  trianglesX;

                //Get the triangle to the left of the above
                triangles[0] = triangles[0] - trianglesX - 1;

                //No more triangles
                triangles[0] =  - 1;
                triangles[0] =  - 1;
                triangles[0] =  - 1;
            }
            else
            {
                //Get triangle one of the square that this vertex is the top left corner of
                triangles[0] = ((j + (i*stepX)) - difference) * 2;

                //If this triangle is the last one in the row, then it doesn't have one to its right
                triangles[1] = ((triangles[0] % (trianglesX - 1)) == 0) ? -1 : (triangles[0] + 1);

                //Get the corresponding triangle in the square above (will check if it's out of bounds later)
                triangles[2] = triangles[0] - trianglesX;

                //If the triangle above is at the start of a row, then there aren't any squares (and thus triangles) left of it
                triangles[3] = (((triangles[0] - trianglesX) % trianglesX) == 0) ? -1 : (triangles[0] - trianglesX - 1);
                triangles[4] = (((triangles[0] - trianglesX) % trianglesX) == 0) ? -1 : (triangles[0] - trianglesX - 2);

                //If triangle is at the start of the row, then it doesn't have one to its left
                triangles[5] = ((triangles[0] % trianglesX) == 0) ? -1 : (triangles[0] - 1);
            }


            for(int k = 0; k < 6; k++)
            {
                //Make sure the triangle within bounds
                if(triangles[k] >= 0 && triangles[k] < mNumberOfIndices/3)
                {
                    pointNormal = pointNormal + normals[triangles[k]];
                }
            }

            pointNormal.normalize();
            mVertices[j + (i*stepX)].setNormal(pointNormal);
        }

        difference++;
    }

    delete[] normals;
    delete[] triangles;
}


//Find the y coordinate based on x and z
float PointCloud::findY(const QVector3D &point, QMatrix4x4 *transformations)
{
    float y = 0.f;
    QVector3D pos = (transformations->inverted() * QVector4D(point, 1.f)).toVector3D();
    int triangle = findPoint(pos);

    if(triangle != -1)
    {
        //https://stackoverflow.com/questions/11262391/from-barycentric-to-cartesian
        QVector3D coord = barycentricCoordinates(mVertices[mIndices[triangle*3]].getPosition(), mVertices[mIndices[(triangle*3)+1]].getPosition(), mVertices[mIndices[(triangle*3)+2]].getPosition(), pos);
        QVector3D carte = coord.x() * mVertices[mIndices[triangle*3]].getPosition() + coord.y() * mVertices[mIndices[(triangle*3)+1]].getPosition() + coord.z() * mVertices[mIndices[(triangle*3)+2]].getPosition();

        y = ((*transformations) * QVector4D(carte, 1.f)).y();
    }

    return y;
}


//Find the triangle a point is in
int PointCloud::findPoint(const QVector3D &point)
{
    bool *triangleVisited = new bool[mNumberOfIndices/3]{false};
    int current = 0;
    bool found = false;
    QVector3D bary(-1.f, -1.f, -1.f);

    //Searches until point is found or it starts backtracking
    while(!found && !triangleVisited[current])
    {
        //Get the barycentric coordinates of the point in relation to the triangle 'current'
        bary = barycentricCoordinates(mVertices[mIndices[current*3]].getPosition(), mVertices[mIndices[(current*3)+1]].getPosition(), mVertices[mIndices[(current*3)+2]].getPosition(), point);
        triangleVisited[current] = true;

        //If point is outside the current triangle
        if(bary.x() < 0.f || bary.y() < 0.f || bary.z() < 0.f)
        {
            float min;
            int index;

            //Find the first point with a neighbor
            if(mNeighbors[current*3] != -1)
            {
                min = bary.x();
                index = 0;
            }
            else if(mNeighbors[(current*3)+1] != -1)
            {
                min = bary.y();
                index = 1;
            }
            else
            {
                min = bary.z();
                index = 2;
            }

            //Check if there's another point with a neighbor where the barycentric coordinate is smaller
            for(int i = 0; i < 3; i++)
            {
                if(bary[i] < min && mNeighbors[(current*3)+i] != -1)
                {
                    min = bary[i];
                    index = i;
                }
            }

            //Next triangle to visit
            current = mNeighbors[(current*3)+index];
        }
        else
        {
            found = true;
        }
    }

    if(found)
    {
        delete[] triangleVisited;
        return current;
    }

    delete[] triangleVisited;
    return -1;
}


//Returns the barycentric coordinates of a point in relation to the given triangle
const QVector3D PointCloud::barycentricCoordinates(const QVector3D &p, const QVector3D &q, const QVector3D &r, const QVector3D &point)
{
    QVector3D temp;

    QVector3D pq = q-p;
    pq.setY(0.f);
    QVector3D pr = r-p;
    pr.setY(0.f);

    QVector3D norm = QVector3D::crossProduct(pq, pr);
    float area = norm.length();

    QVector3D pointP = p-point;
    pointP.setY(0.f);
    QVector3D pointQ = q-point;
    pointQ.setY(0.f);
    QVector3D pointR = r-point;
    pointR.setY(0.f);

    temp.setX(QVector3D::crossProduct(pointQ, pointR).y()/area);
    temp.setY(QVector3D::crossProduct(pointR, pointP).y()/area);
    temp.setZ(QVector3D::crossProduct(pointP, pointQ).y()/area);

    return temp;
}


void PointCloud::init()
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
}
