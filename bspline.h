#ifndef BSPLINE_H
#define BSPLINE_H

#include <QVector3D>
#include <vector>

class ObjectInstance;

class BSpline
{
public:
    BSpline();
    ~BSpline();

    void addPoint(ObjectInstance *p);
    void clearPoints();
    void reorderPoints(QVector3D startPoint);
    QVector3D findPoint(float t);
    void makeKnots();
    QVector3D deBoor(float t, unsigned int interval);
    int findKnotInterval(float t);
    void setDegree(unsigned int degree) {mDegree = degree;}

private:
    std::vector<ObjectInstance*> mTrophies;
    std::vector<QVector3D> mControlPoints;
    std::vector<float> mKnots;
    int mNumberOfKnots;
    unsigned int mDegree = 3;
};

#endif // BSPLINE_H
