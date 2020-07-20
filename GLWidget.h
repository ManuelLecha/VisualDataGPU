#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <math.h>
#include <Camera.h>
#include <QtWidgets>
#include <QGLWidget>
#include <QtOpenGL>
#include <QTime>

#include <scenes/Scene.h>

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_COLOR_ATTRIBUTE 1

class QGLShaderProgram;

enum class SHADER_TYPE { GOURAND, PHONG, TOON, TEXTURE, BACKGROUND, BUMP_MAPPING, PLANE, BG_REFLECTION, BG_REFRACTION};
enum class PAINT_GL_BEHAVIOUR {DEFAULT, BACKGROUND, DATA, DIFFERENT_SHADING};
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    GLWidget(const QGLFormat &glf, QWidget *parent=0);
    ~GLWidget();
    PAINT_GL_BEHAVIOUR current_behaviour = PAINT_GL_BEHAVIOUR::DEFAULT;
    PAINT_GL_BEHAVIOUR previous_behaviour = PAINT_GL_BEHAVIOUR::DEFAULT;
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setScene (Scene *sc);
    Scene *getScene() {return scene;}

public slots:

    void updateObject(Object *obj);
    void updateScene(Scene *sc);

    void saveImage();
    void saveAnimation();

    void activaToonShader();
    void activaPhongShader();
    void activaGouraudShader();

    void activaPhongTex();
    void activaBackground();
    void activaBumpMapping();
    void activaEnvMapping();
    void activaTransparency();
    void activaDadesMultipleShading();

    void activaBGTransparency();
    void activaBGReflection();

    void setRotation(float angle, float x, float y, float z);
    void setTranslation(float x, float y, float z);
    void setScale(float s);
    void setPerspective(float verticalAngle, float nearPlane, float farPlane);
    void setLookAt(const QVector3D &eye, const QVector3D &center, const QVector3D& up);
    void setLighting(const QVector3D &lightPos, const QVector3D &Ia, const QVector3D &Id,
                               const QVector3D &Is, const QVector3D &coefs);
    void setTextureFile(const QString &file);
    void setNormalTextureFile(const QString &file);
    void setBackgroundTextureFile(const QString &file);

    // Acció per activar a cada update del timer
    void setCurrentFrame();

    void newData(Scene *sc);
    void newScene(Scene *sc);
    void startDemo1(Scene *sc);
    void startDemo2(Scene *sc);
    void animateCamera();

signals:
    void ObsCameraChanged(Camera *cam);
    void FrustumCameraChanged(Camera *cam);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void setXRotation(int angle);
    void setYRotation(int angle);

    void qNormalizeAngle(double &angle);

    void Zoom (int positiu);
    void saveFrame();

private:
    Scene*  scene; // Escena que es pinta al GLWidget

    QTimer *timer;
    unsigned int currentFrame;
    unsigned int currentImage;

    QPoint lastPos;   // per interactuar amb la camera

    map<SHADER_TYPE, QGLShaderProgram*> programs; // Per ars nomes es té un parell vertex-fragment
    QGLShaderProgram* currentProgram;
    SHADER_TYPE currentShader;
    bool backgroundActivated = false;

    void setCurrentShaderAndBind(SHADER_TYPE shaderToBind);
    void updateShader();
    void updateShaderTexture();
    void updateShaderBump();
    void createProgramsShaders();
    void setCurrentProgramShaderToDefault();
    void executeDrawForCurrentShader();
    void executeDrawForCurrentShader(QGLShaderProgram* program);
    void executeDrawForBackground();
    QGLShaderProgram* createProgram(const char* vShaderFile, const char* fShaderFile);
    void renderFrame();
};

#endif // GLWIDGET_H
