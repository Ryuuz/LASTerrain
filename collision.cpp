#include "collision.h"
#include "boundingvolume.h"
#include "objectinstance.h"
#include "physicsobject.h"
#include "constants.h"
#include "pointcloud.h"
#include "transform.h"
#include <vector>
#include <QVector3D>
#include <QDebug>


Collision::Collision()
{

}


Collision::~Collision()
{

}


bool Collision::collisionDetection(BoundingVolume *obj1, BoundingVolume *obj2)
{
    //If sphere vs box
    if(obj1->getType() == BoundType::sphere && (obj2->getType() == BoundType::box || obj2->getType() == BoundType::trigger))
    {
        //Center and radius of sphere
        QVector3D sphereCenter = ((obj1->getMax() + obj1->getMin()) / 2.f);
        float sphereRadius = (obj1->getMax() - sphereCenter).lengthSquared();

        //https://stackoverflow.com/questions/4578967/cube-sphere-intersection-test
        float minDist = 0.f;

        for(int i = 0; i < 3; i++)
        {
            if(sphereCenter[i] < obj2->getMin()[i])
            {
                minDist += std::pow(sphereCenter[i] - obj2->getMin()[i], 2);
            }
            else if(sphereCenter[i] > obj2->getMax()[i])
            {
                minDist += std::pow(sphereCenter[i] - obj2->getMax()[i], 2);
            }
        }

        return minDist <= sphereRadius;
    }
    //Else if sphere vs plane
    else if(obj1->getType() == BoundType::sphere && obj2->getType() == BoundType::plane)
    {
        //Center and radius of sphere
        QVector3D sphereCenter = ((obj1->getMax() + obj1->getMin()) / 2.f);
        float sphereRadius = (obj1->getMax() - sphereCenter).length();

        //Third point on plane found by the two points already known
        QVector3D point3;
        point3.setX(obj2->getMax().x());
        point3.setY(obj2->getMin().y());
        point3.setZ(obj2->getMax().z());

        //Plane normal
        QVector3D normal = QVector3D::crossProduct((obj2->getMax() - point3), (obj2->getMin() - point3));
        normal.normalize();

        //Distance between sphere center and plane
        float dist = QVector3D::dotProduct((sphereCenter - point3), normal);

        if(std::abs(dist) <= sphereRadius)
        {
            return true;
        }
    }
    //Else if sphere vs sphere
    else if(obj1->getType() == BoundType::sphere && obj2->getType() == BoundType::sphere)
    {
        QVector3D sphereCenter1 = ((obj1->getMax() + obj1->getMin()) / 2.f);
        float sphereRadius1 = (obj1->getMax() - sphereCenter1).length();

        QVector3D sphereCenter2 = ((obj2->getMax() + obj2->getMin()) / 2.f);
        float sphereRadius2 = (obj2->getMax() - sphereCenter2).length();

        if((sphereCenter2 - sphereCenter1).length() <= (sphereRadius1 + sphereRadius2))
        {
            return true;
        }
    }

    return false;
}


//One object with physics
void Collision::collisionHandling(PhysicsObject *obj1, ObjectInstance *obj2)
{
    if(obj1->getTag() == gsl::player && obj2->getTag() == gsl::trophy)
    {
        obj2->setDestroyed(true);
    }
    else if(obj2->getBounds()->getType() == BoundType::box && obj1->getTag() != gsl::enemy)
    {
        obj1->setVelocity(obj1->getVelocity()*(-0.5f));
    }
    else if(obj2->getBounds()->getType() == BoundType::plane)
    {
        //Find normal
        QVector3D point3;
        point3.setX(obj2->getBounds()->getMax().x());
        point3.setY(obj2->getBounds()->getMin().y());
        point3.setZ(obj2->getBounds()->getMax().z());

        QVector3D normal = QVector3D::crossProduct((obj2->getBounds()->getMax() - point3), (obj2->getBounds()->getMin() - point3));
        normal.normalize();

        //Mirror old velocity around plane normal to get new velocity
        QVector3D newVel = obj1->getVelocity() - 2 * QVector3D::dotProduct(obj1->getVelocity(), normal)*normal;
        obj1->setVelocity(newVel);
    }
}


//Two objects with physics
void Collision::collisionHandling(PhysicsObject *obj1, PhysicsObject *obj2)
{
    //https://www.gamasutra.com/view/feature/131424/pool_hall_lessons_fast_accurate_.php?page=3
    if(obj1->getBounds()->getType() == BoundType::sphere && obj2->getBounds()->getType() == BoundType::sphere)
    {
        QVector3D sphere1Center = ((obj1->getBounds()->getMax() + obj1->getBounds()->getMin()) / 2.f);
        QVector3D sphere2Center = ((obj2->getBounds()->getMax() + obj2->getBounds()->getMin()) / 2.f);

        //Distance between the two spheres
        QVector3D n = sphere1Center - sphere2Center;
        n.normalize();

        float a1 = QVector3D::dotProduct(obj1->getVelocity(), n);
        float a2 = QVector3D::dotProduct(obj2->getVelocity(), n);

        float p = (2.f*(a1-a2))/2;

        obj1->setVelocity(obj1->getVelocity() - p * n);
        obj2->setVelocity(obj2->getVelocity() + p * n);
    }
}


void Collision::groundCollision(PhysicsObject *obj1, ObjectInstance *ground)
{
    QVector3D pos = ((obj1->getBounds()->getMax() + obj1->getBounds()->getMin()) / 2.f);
    PointCloud *groundMesh = dynamic_cast<PointCloud*>(ground->getModel());
    int current = groundMesh->findPoint(pos, ground->getModelMatrix());
    QVector3D yCoordinates = groundMesh->findY(pos, ground->getModelMatrix(), current);

    float radius = (obj1->getBounds()->getMax() - pos).length();
    float distance = (pos - yCoordinates).length();

    if(current != -1)
    {
        if(obj1->mGrounded)
        {
            if(distance <= (radius + 0.05f) && current != -1)
            {
                obj1->setYPos(yCoordinates.y() + radius);
                obj1->setTriangleNormal(groundMesh->getTriangle(current).norm);
            }
            else
            {
                obj1->mGrounded = false;
            }
        }
        else
        {
            if(distance <= (radius + 0.1f) && current != -1)
            {
                obj1->mGrounded = true;
                obj1->setYPos(yCoordinates.y() + radius);
                obj1->setTriangleNormal(groundMesh->getTriangle(current).norm);
                obj1->setVelocity(obj1->getVelocity() - ((obj1->getTriangleNormal() * 2.f * QVector3D::dotProduct(obj1->getVelocity(), obj1->getTriangleNormal())) * 0.8f));
            }
        }

        if(current != obj1->mCurrentTriangle && obj1->mGrounded)
        {
            QVector3D collisionNormal;

            collisionNormal = (obj1->getTriangleNormal() + groundMesh->getTriangle(current).norm) * (1 / (obj1->getTriangleNormal().length() + groundMesh->getTriangle(current).norm.length()));
            collisionNormal.normalize();

            obj1->setVelocity(obj1->getVelocity() - (collisionNormal * 2.f * QVector3D::dotProduct(obj1->getVelocity(), collisionNormal)));
        }
    }

    obj1->mCurrentTriangle = current;
}
