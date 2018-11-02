#include "renderwindow.h"

#include <QTimer>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QDebug>

#include "constants.h"
#include "mainwindow.h"
#include "shader.h"
#include "material.h"
#include "colormaterial.h"
#include "lightmaterial.h"
#include "light.h"
#include "camera.h"

#include "sceneobject.h"
#include "objectinstance.h"
#include "physicsobject.h"
#include "player.h"
#include "npc.h"

#include "sphere.h"
#include "axis.h"
#include "pointcloud.h"
#include "cube.h"

#include "boundingvolume.h"
#include "transform.h"
#include "collision.h"
#include "bspline.h"


RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext{nullptr}, mInitialized{false}, mMainWindow{mainWindow}
{
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    mContext = new QOpenGLContext(this);
    mContext->setFormat(requestedFormat());
    if (!mContext->create())
    {
        delete mContext;
        mContext = nullptr;
    }
}


RenderWindow::~RenderWindow()
{
    removeObjects();

    mCurrentCamera = nullptr;
    delete mCamera1;

    delete mAxis;
    delete mTerrain;
    delete mSphere;
    delete mCube;

    delete mLightMaterial;
    delete mMaterial;
    delete mLight;

    delete mColorShader;
    delete mLightShader;

    delete mCollision;
    delete mSpline;
}


void RenderWindow::init()
{
    if (!mContext->makeCurrent(this)) {
        emit error(tr("makeCurrent() failed"));
        return;
    }

    if (!mInitialized)
    {
        mInitialized = true;
    }

    initializeOpenGLFunctions();

    startOpenGLDebugger();

    mCamera1 = new Camera;
    mCamera1->getTransform()->setPosition(90.f, 100.f, 100.f);

    //Set the perspective and view matrices
    mCamera1->setPerspectiveMatrix(width(), height());
    mCurrentCamera = mCamera1;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    //Compile shaders:
    mColorShader = new Shader("vertexshader.vert", "fragmentshader.frag");
    mLightShader = new Shader("lightshader.vert", "lightshader.frag");

    mLight = new Light(QVector3D(0.f, 100.f, 0.f));
    mMaterial = new ColorMaterial(mColorShader);
    mLightMaterial = new LightMaterial(mLightShader, mLight);

    Material::setCamera(mCurrentCamera);

    mCollision = new Collision;

    //Load assets
    mAxis = new Axis;
    mAxis->setMaterial(mMaterial);
    glBindVertexArray(0);

    mTerrain = new PointCloud("terrain.las", 0.6f, 0.8f);
    mTerrain->setMaterial(mLightMaterial);
    glBindVertexArray(0);

    mSphere = new Sphere(3);
    mSphere->setMaterial(mLightMaterial);
    glBindVertexArray(0);

    mCube = new Cube;
    mCube->setMaterial(mLightMaterial);
    glBindVertexArray(0);

    //Create and place objects
    createObjects();

    mCamera1->setTarget(QVector3D(0.f, 0.f, 0.f));

    emit ready();   //tell the mainWindow that init is finished
}


void RenderWindow::createObjects()
{
    ObjectInstance *obj;
    PhysicsObject *physObj;
    mSpline = new BSpline;

    //Axis
    obj = new ObjectInstance(mAxis);
    mStaticObjects.push_back(obj);

    //Terrain
    obj = new ObjectInstance(mTerrain);
    obj->getTransform()->setPosition(-100.f, -90.f, -50.f);
    obj->getTransform()->setScale(0.5f, 0.5f, 0.5f);
    obj->setColor(QVector3D(0.1f, 0.8f, 0.2f));
    mStaticObjects.push_back(obj);
    mWorldTerrain = mStaticObjects.back(); 

    //Spheres
    physObj = new Player(mSphere);
    physObj->setColor(QVector3D(1.f, 0.f, 0.f));
    physObj->setBoundingObject(BoundType::sphere);
    physObj->getTransform()->setPosition(-20.f, 10.f, 0.f);
    mDynamicObjects.push_back(physObj);
    mPlayer = dynamic_cast<Player*>(mDynamicObjects.back());

    physObj = new NPC(mSphere, mSpline);
    physObj->setColor(QVector3D(0.f, 0.f, 1.f));
    physObj->setBoundingObject(BoundType::sphere);
    physObj->getTransform()->setPosition(30.f, 0.f, -15.f);
    physObj->setTag(gsl::enemy);
    mDynamicObjects.push_back(physObj);

    //Starting point
    obj = new ObjectInstance(nullptr);
    obj->getTransform()->setPosition(mDynamicObjects.back()->getTransform()->getTranslation().x(),
                                     mDynamicObjects.back()->getTransform()->getTranslation().y(),
                                     mDynamicObjects.back()->getTransform()->getTranslation().z());
    mStaticObjects.push_back(obj);
    mSpline->addPoint(mStaticObjects.back());

    //Cubes
    obj = new ObjectInstance(mCube);
    obj->setColor(QVector3D(0.3f, 0.f, 1.f));
    obj->setTag(gsl::trophy);
    obj->setBoundingObject(BoundType::box);
    obj->getTransform()->setPosition(30.f, 2.f, 2.f);
    mStaticObjects.push_back(obj);
    mSpline->addPoint(mStaticObjects.back());

    obj = new ObjectInstance(mCube);
    obj->setColor(QVector3D(0.3f, 0.f, 1.f));
    obj->setTag(gsl::trophy);
    obj->setBoundingObject(BoundType::box);
    obj->getTransform()->setPosition(5.f, 2.f, -20.f);
    mStaticObjects.push_back(obj);
    mSpline->addPoint(mStaticObjects.back());

    obj = new ObjectInstance(mCube);
    obj->setColor(QVector3D(0.3f, 0.f, 1.f));
    obj->setTag(gsl::trophy);
    obj->setBoundingObject(BoundType::box);
    obj->getTransform()->setPosition(-20.f, 2.f, 15.f);
    mStaticObjects.push_back(obj);
    mSpline->addPoint(mStaticObjects.back());

    //Ending point
    obj = new ObjectInstance(nullptr);
    obj->getTransform()->setPosition(-30.f, 2.f, 10.f);
    mStaticObjects.push_back(obj);
    mSpline->addPoint(mStaticObjects.back());

    obj = nullptr;
    physObj = nullptr;

    //mSpline->createBSpline();
}


//Delete objects that are marked as destroyed
void RenderWindow::cleanUpObjects()
{
    for(int i = static_cast<int>(mStaticObjects.size()-1); i >= 0; i--)
    {
        if(mStaticObjects.at(static_cast<unsigned int>(i))->isDestroyed())
        {
            delete mStaticObjects.at(static_cast<unsigned int>(i));
            mStaticObjects.at(static_cast<unsigned int>(i)) = nullptr;
            mStaticObjects.erase(mStaticObjects.begin() + i);
        }
    }

    for(int i = static_cast<int>(mDynamicObjects.size()-1); i >= 0; i--)
    {
        if(mDynamicObjects.at(static_cast<unsigned int>(i))->isDestroyed())
        {
            delete mDynamicObjects.at(static_cast<unsigned int>(i));
            mDynamicObjects.at(static_cast<unsigned int>(i)) = nullptr;
            mDynamicObjects.erase(mDynamicObjects.begin() + i);
        }
    }
}


//Delete all objects from the scene
void RenderWindow::removeObjects()
{
    mPlayer = nullptr;

    for(auto element : mStaticObjects)
    {
        delete element;
    }
    mStaticObjects.clear();

    for(auto element : mDynamicObjects)
    {
        delete element;
    }
    mDynamicObjects.clear();
}


void RenderWindow::render()
{
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Draw the models
    for(auto object : mStaticObjects)
    {
        object->draw();
        checkForGLerrors();
    }

    for(auto object : mDynamicObjects)
    {
        object->draw();
        checkForGLerrors();
    }

    glBindVertexArray(0);
    mContext->swapBuffers(this);
}


void RenderWindow::update()
{
    for(auto object : mDynamicObjects)
    {
        //Move object
        object->physicsUpdate(mDeltaTime);
        mCollision->groundCollision(object, mWorldTerrain);

        for(auto item : mStaticObjects)
        {
            if(object->getBounds() && item->getBounds() && !item->isDestroyed())
            {
                if(mCollision->collisionDetection(object->getBounds(), item->getBounds()))
                {
                    mCollision->collisionHandling(object, item);
                }
            }
        }

        //Check object against other physics objects for collision
        for(auto item : mDynamicObjects)
        {
            if(item != object && item->getBounds() && object->getBounds())
            {
                if(mCollision->collisionDetection(object->getBounds(), item->getBounds()))
                {
                    mCollision->collisionHandling(object, item);
                }
            }
        }
    }

    //cleanUpObjects();
}


void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        qDebug() << "Can this system run QOpenGLDebugLogger? :" << format.testOption(QSurfaceFormat::DebugContext);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
            {
                qDebug() << "Started OpenGL debug logger!";
            }
        }
    }
}


void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << "glGetError returns " << err;
        }
    }
}


//This function is called from Qt when window is exposed / shown
void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (!mInitialized)
    {
       init();
    }

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    if (isExposed())
    {
        mTimer.start(16, this);
        mTimeStart.start();
    }
}


void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
            mMainWindow->close();
    }
    else
    {
        mPlayer->handleInput(event, true);
    }
}


void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    mPlayer->handleInput(event, false);
}


void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        mMouseX = event->pos().x();
        mMouseY = event->pos().y();
        mRotateCamera = true;
    }
}


void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        mRotateCamera = false;
    }
}


void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(mRotateCamera)
    {
        float rotationX = (event->pos().x() - mMouseX);

        mCamera1->getTransform()->rotate(0.f, rotationX, 0.f);
        mCamera1->setViewMatrix();

        mMouseX = event->pos().x();
        mMouseY = event->pos().y();
    }
}


void RenderWindow::timerEvent(QTimerEvent *)
{
    update();
    render();

    mDeltaTime = mTimeStart.restart() / 1000.f;
}
