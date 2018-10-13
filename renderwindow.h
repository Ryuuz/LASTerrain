#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QBasicTimer>
#include <QTime>
#include <vector>

class QOpenGLContext;
class MainWindow;
class Shader;
class Material;
class Light;
class Camera;
class ObjectInstance;
class PhysicsObject;
class Collision;

class Sphere;
class Axis;
class PointCloud;


class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT

    public:
        RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
        ~RenderWindow() override;
        QOpenGLContext *context() { return mContext; }
        void exposeEvent(QExposeEvent *) override;

    signals:
        void ready();
        void error(const QString &msg);

    private slots:
        void render();
        void update();

    private:
        void init();
        void createObjects();
        void removeObjects();
        void startOpenGLDebugger();

        QOpenGLContext *mContext;
        bool mInitialized;

        Shader *mColorShader;
        Shader *mLightShader;

        Material *mMaterial;
        Material *mLightMaterial;
        Light *mLight;

        Camera *mCurrentCamera;
        Camera *mCamera1;

        Collision *mCollision;

        Axis *mAxis;
        Sphere *mSphere;
        PointCloud *mTerrain;

        std::vector<ObjectInstance*> mStaticObjects;
        std::vector<PhysicsObject*> mDynamicObjects;

        ObjectInstance *mWorldTerrain;
        PhysicsObject *mPlayer;

        float mDeltaTime{0.f};
        QBasicTimer mTimer;     //timer that drives the gameloop
        QTime mTimeStart;       //time variable that reads the actual FPS

        MainWindow *mMainWindow;

        bool mRotateCamera{false};
        int mMouseX;
        int mMouseY;

        class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

        void checkForGLerrors();

    protected:
        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void timerEvent(QTimerEvent *) override;
};

#endif // RENDERWINDOW_H
