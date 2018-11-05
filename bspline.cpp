#include "bspline.h"
#include "objectinstance.h"
#include "transform.h"
#include <algorithm>
#include <QRandomGenerator>


BSpline::BSpline()
{

}


BSpline::~BSpline()
{

}


void BSpline::addPoint(ObjectInstance *p)
{
    mTrophies.push_back(p);
}


void BSpline::clearPoints()
{
    mControlPoints.clear();
}


void BSpline::reorderPoints(QVector3D startPoint)
{
    mControlPoints.clear();

    //Remove trophies that have been picked up
    for(unsigned int i = 0; i < mTrophies.size(); i++)
    {
        if(mTrophies.at(i)->isDestroyed())
        {
            mTrophies.erase(mTrophies.begin() + static_cast<int>(i));
        }
    }

    //Update start and end points
    std::swap(mTrophies.back(), mTrophies.front());
    mControlPoints.push_back(startPoint/*mTrophies.front()->getPosition()*/);

    //Swap the order of the objects
    unsigned int index = 0;
    for(unsigned int i = 1; i < (mTrophies.size() - 1); i++)
    {
        index = QRandomGenerator::global()->bounded(mTrophies.size() - 2) + 1;
        if(index != i)
        {
            std::swap(mTrophies.at(i), mTrophies.at(index));
        }
    }

    //Add the objects as control points
    for(unsigned int i = 1; i < (mTrophies.size() - 1); i++)
    {
        mControlPoints.push_back(mTrophies.at(i)->getPosition());
    }

    mControlPoints.push_back(mTrophies.back()->getPosition());
}


QVector3D BSpline::findPoint(float t)
{
    QVector3D curvePoint;

    if(mControlPoints.size() > 2)
    {
        int knotSpan = findKnotInterval(t);

        if(knotSpan != -1)
        {
            curvePoint = deBoor(t, static_cast<unsigned int>(knotSpan));
        }
    }

    return curvePoint;
}


//Generate knots based on number of points and degree
void BSpline::makeKnots()
{
    mKnots.clear();
    unsigned int knots = (mControlPoints.size() - 1) + mDegree + 1;
    unsigned int middleKnots = (knots + 1) - (mDegree + 1) * 2;

    float knotStep = 1.f / static_cast<float>(middleKnots + 1);

    for(unsigned int i = 0; i < (knots + 1); i++)
    {
        if(i < mDegree + 1)
        {
            mKnots.push_back(0.f);
        }
        else if(i < (knots - mDegree))
        {
            mKnots.push_back(mKnots.back() + knotStep);
        }
        else
        {
            mKnots.push_back(1.f);
        }
    }
}


QVector3D BSpline::deBoor(float t, unsigned int interval)
{
    QVector3D *a = new QVector3D[mDegree+1];
    float w;

    for(unsigned int j = 0; j <= mDegree; j++)
    {
        a[mDegree-j] = mControlPoints[interval-j];
    }
    for(unsigned int d = mDegree; d >= 1; d--)
    {
        unsigned int j = interval - d;
        for(unsigned int i = 0; i < d; i++)
        {
            j = j + 1;
            w = (t - mKnots[j]) / (mKnots[j+d] - mKnots[j]);
            a[i] = ((1 - w) * a[i]) + (w * a[i+1]);
        }
    }

    QVector3D temp = a[0];
    delete[] a;

    return temp;
}


int BSpline::findKnotInterval(float t)
{
    bool spanFound = false;
    int knotSpan = -1;

    for(unsigned int i = 1; i < mKnots.size(); i++)
    {
        if(!spanFound && t >= mKnots[i-1] && t < mKnots[i])
        {
            knotSpan = static_cast<int>(i - 1);
            spanFound = true;
        }
    }

    return knotSpan;
}
