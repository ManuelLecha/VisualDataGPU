#include <math.h>
#include <QtDebug>
#include <QString>
#include "GLWidget.h"
#include "DataAnimation.h"
#include "./animations/Animation.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
    setFocusPolicy( Qt::StrongFocus );
}

GLWidget::GLWidget(const QGLFormat &glf, QWidget *parent) : QGLWidget(glf, parent) {
    setFocusPolicy( Qt::StrongFocus );
    scene = new Scene();
    scene->setCamera(new Camera(this->size().width(), this->size().height()));
    emit ObsCameraChanged(scene->camera);
    emit FrustumCameraChanged(scene->camera);
}

GLWidget::~GLWidget() {
    makeCurrent();
    delete scene;
}

void GLWidget::createProgramsShaders() {
    programs.insert(make_pair(SHADER_TYPE::GOURAND, createProgram("://resources/GourandVShader.glsl", "://resources/GourandFShader.glsl")));
    programs.insert(make_pair(SHADER_TYPE::PHONG, createProgram("://resources/PhongVShader.glsl", "://resources/PhongFShader.glsl")));
    programs.insert(make_pair(SHADER_TYPE::TOON, createProgram("://resources/ToonVShader.glsl", "://resources/ToonFShader.glsl")));
    programs.insert(make_pair(SHADER_TYPE::TEXTURE, createProgram("://resources/PhongTexVShader.glsl", "://resources/PhongTexFShader.glsl")));
    programs.insert(make_pair(SHADER_TYPE::BUMP_MAPPING, createProgram("://resources/BumpMappingVShader.glsl", "://resources/BumpMappingFShader.glsl")));
    programs.insert(make_pair(SHADER_TYPE::BACKGROUND,createProgram("://resources/BackgroundVShader.glsl", "://resources/BackgroundFShader.glsl")));
    programs.insert(make_pair(SHADER_TYPE::BG_REFLECTION,createProgram("://resources/BGReflexionVShader.glsl", "://resources/BGReflexionFShader.glsl")));
    programs.insert(make_pair(SHADER_TYPE::PLANE,createProgram("://resources/planevshader.glsl", "://resources/planefshader.glsl")));
    programs.insert(make_pair(SHADER_TYPE::BG_REFRACTION, createProgram("://resources/BGRefractionVShader.glsl", "://resources/BGRefractionFShader.glsl")));
}

QGLShaderProgram* GLWidget::createProgram(const char* vShaderFile, const char* fShaderFile) {
    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);

    vshader->compileSourceFile(vShaderFile);
    fshader->compileSourceFile(fShaderFile);

    QGLShaderProgram *program = new QGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);
    program->link();
    return program;
}

void GLWidget::setScene(Scene *sc) {
    scene = sc;
    scene->setCamera(new Camera(this->size().width(), this->size().height()));
    emit ObsCameraChanged(scene->camera);
    emit FrustumCameraChanged(scene->camera);
}

/* Interacció amb GL i inicialització de shaders */

/**
 * @brief GLWidget::initializeGL()
 */
void GLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_RGBA);
    glEnable(GL_DOUBLE);

    createProgramsShaders();
    setCurrentProgramShaderToDefault();

    //Lights
    Light *light0 = new Light(Puntual);
    light0->setIa(vec3(0.2,0.2,0.2));
    light0->setId(vec3(0.8,0.8,0.8));
    light0->setIs(vec3(1.0,1.0,1.0));
    light0->setLightPosition(vec3(10,10,20));
    light0->setCoeficients(vec3(1,0,0));
    scene->addLight(light0);

    //Ambient scene light
    scene->setLightGlobalAmbient(vec3(0.9,0.9,0.9));

    scene->camera->init(this->size().width(), this->size().height(), scene->capsaMinima);
    scene->camera->setFrustum(120, 0.01, 100);
    scene->capsaBackground->make();

    emit ObsCameraChanged(scene->camera);
    emit FrustumCameraChanged(scene->camera);

    glViewport(scene->camera->vp.pmin[0], scene->camera->vp.pmin[1], scene->camera->vp.a, scene->camera->vp.h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief GLWidget::paintGL()
 */
void GLWidget::paintGL() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    if(current_behaviour == PAINT_GL_BEHAVIOUR::DEFAULT){
        scene->camera->toGPU(currentProgram);
        executeDrawForCurrentShader();
    }else if (current_behaviour == PAINT_GL_BEHAVIOUR::BACKGROUND){
        SHADER_TYPE backup_shader_type = currentShader;

        setCurrentShaderAndBind(SHADER_TYPE::BACKGROUND);
        scene->camera->toGPU(currentProgram);
        scene->renderBackground(currentProgram);

        setCurrentShaderAndBind(backup_shader_type);
        scene->camera->toGPU(currentProgram);
        executeDrawForCurrentShader();

    }else if (current_behaviour == PAINT_GL_BEHAVIOUR::DIFFERENT_SHADING && previous_behaviour == PAINT_GL_BEHAVIOUR::DATA){
        qDebug()<<"si que entra";
        SHADER_TYPE variable_shader_type;
        SHADER_TYPE backup_shader_type = currentShader;

        setCurrentShaderAndBind(SHADER_TYPE::PLANE);
        scene->camera->toGPU(currentProgram);
        scene->renderPlane(currentProgram);

        for (int i = 0; i<scene->elements.size(); i++){
            if (variable_shader_type == SHADER_TYPE::GOURAND){
                variable_shader_type = SHADER_TYPE::TOON;
            }else if (variable_shader_type == SHADER_TYPE::TOON){
                variable_shader_type = SHADER_TYPE::PHONG;
            }else{
               variable_shader_type = SHADER_TYPE::GOURAND;
            }
            setCurrentShaderAndBind(variable_shader_type);
            scene->camera->toGPU(currentProgram);
            scene->lightsToGPU(currentProgram);
            scene->setAmbientToGPU(currentProgram);
            scene->elements.at(i)->toGPU(currentProgram);
            scene->elements.at(i)->draw();
        }

    }else if(current_behaviour == PAINT_GL_BEHAVIOUR::DATA){
        SHADER_TYPE backup_shader_type = currentShader;

        setCurrentShaderAndBind(SHADER_TYPE::PLANE);
        scene->camera->toGPU(currentProgram);
        scene->renderPlane(currentProgram);

        setCurrentShaderAndBind(backup_shader_type);
        scene->camera->toGPU(currentProgram);
        executeDrawForCurrentShader();
    }else{
        current_behaviour = PAINT_GL_BEHAVIOUR::DEFAULT;
    }
}

void GLWidget::executeDrawForCurrentShader() {
    if(currentShader == SHADER_TYPE::TEXTURE){
        scene->drawTexture();
    }else if(currentShader == SHADER_TYPE::BUMP_MAPPING){
        scene->drawNormalTexture();
    }else if(currentShader == SHADER_TYPE::BG_REFLECTION){
        scene->drawBackgroundReflection();
    }else if(currentShader == SHADER_TYPE::BG_REFRACTION){
        scene->drawBackgroundRefraction();
    }else{
        scene->draw();
    }
}


void GLWidget::executeDrawForBackground() {
    SHADER_TYPE backup_shader_type = currentShader;
    setCurrentShaderAndBind(SHADER_TYPE::BACKGROUND);
    scene->renderBackground(currentProgram);
    //setCurrentShaderAndBind(backup_shader_type);
}
/**
 * @brief GLWidget::resizeGL()
 */
void GLWidget::resizeGL(int width, int height) {

    glViewport(scene->camera->vp.pmin[0], scene->camera->vp.pmin[1], scene->camera->vp.a, scene->camera->vp.h);

    scene->camera->vp.a = width;
    scene->camera->vp.h = height;

    scene->camera->CalculaWindow(scene->capsaMinima);

    updateGL();
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(400, 400);
}

void GLWidget::setCurrentProgramShaderToDefault() {
    setCurrentShaderAndBind(SHADER_TYPE::GOURAND);
}

void GLWidget::setCurrentShaderAndBind(SHADER_TYPE shaderToBind){
    QGLShaderProgram* programShaderSelected = programs.at(shaderToBind);
    currentShader = shaderToBind;
    currentProgram = programShaderSelected;
    cout << "=======================" <<endl;
    cout << "Program id type: " << currentProgram->programId() <<endl;
    cout << "Added correctly:" << currentProgram->bind() << endl;
    cout << "=======================" <<endl;
}

/** Gestio de les animacions i la gravació d'imatges ***/

void GLWidget::setCurrentFrame(){
    scene->update(currentFrame);
    renderFrame();
    this->saveFrame();
    currentFrame++;
    if (currentFrame == MAXFRAMES){
        timer->stop();
    }
}

void GLWidget::saveFrame(){
    QString s = QString("screen_output_%1.jpg").arg(currentFrame, 4, 10, QChar('0'));
    this->grabFrameBuffer().save(s);
}

void GLWidget::saveImage(){
    this->grabFrameBuffer().save("screen_output_"+QVariant(currentImage).toString()+".jpg");
    currentImage++;
}


/** Metodes SLOTS que serveixen al builder per a actualitzar l'escena i els objectes */

void GLWidget::startDemo1(Scene *sc){
    qDebug()<<"Estic a Demo 1";
    current_behaviour = PAINT_GL_BEHAVIOUR::BACKGROUND;
    setCurrentShaderAndBind(SHADER_TYPE::BUMP_MAPPING);
    scene->camera->setFrustum(120, 0.01, 100);
    updateScene(sc);
    timer = new QTimer(this);
    currentFrame=0;
    currentImage=0;
    connect(timer, SIGNAL(timeout()), this, SLOT(animateCamera()));
    timer->start(25);
}

void GLWidget::animateCamera(){
    qDebug()<<"Frame "<<currentFrame;
    float angle = -((float)currentFrame)/MAXFRAMES * PI * 2.0f;
    float r = 1.25;
    vec3 center = vec3(0,0,0);
    float x = cos(angle)*r;
    float z = sin(angle)*r;
    vec3 pos = vec3(x,0,z);
    scene->camera->setFrustum(100, 0.01, 100);
    scene->camera->setObservador( vec4(pos, 1), vec4(center, 1), vec4(0,1,0,1));

    renderFrame();
    this->saveFrame();
    currentFrame++;
    if (currentFrame == MAXFRAMES){
        timer->stop();
    }
}

void GLWidget::startDemo2(Scene *sc){
    qDebug()<<"Estic a Demo 1";
    current_behaviour = PAINT_GL_BEHAVIOUR::BACKGROUND;
    updateScene(sc);
    timer = new QTimer(this);
    currentFrame=0;
    currentImage=0;
    connect(timer, SIGNAL(timeout()), this, SLOT(animateCamera()));
    timer->start(25);
}

void GLWidget::updateObject(Object *obj) {

    obj->toGPU(currentProgram);

    emit ObsCameraChanged(scene->camera);
    emit FrustumCameraChanged(scene->camera);

    updateGL();
}

void GLWidget::updateScene(Scene *sc) {
    // Control de la nova escena a la GPUBump Mapping

    scene = sc;
    scene->toGPU(currentProgram);

    // Podeu Utilitzar l'altre constructor de Camera
    // per inicialitzar els parametres de la camara.
    scene->setCamera(new Camera(this->size().width(), this->size().height()));
    scene->camera->actualitzaCamera(scene->capsaMinima);

    emit ObsCameraChanged(scene->camera);
    emit FrustumCameraChanged(scene->camera);

    updateGL();

}

/** Metodes que es criden des dels menús */

void GLWidget::saveAnimation() {
    // Comença el timer de l'animació
    DataAnimation dataAnimation = DataAnimation();
    renderFrame();
    timer = new QTimer(this);
    currentFrame=0;
    currentImage=0;
    connect(timer, SIGNAL(timeout()), this, SLOT(setCurrentFrame()));
    timer->start(33);
}

void GLWidget::activaToonShader() {
    setCurrentShaderAndBind(SHADER_TYPE::TOON);
    updateShader();
    qDebug()<<"Estic a Toon";
}

void GLWidget::activaPhongShader() {
    setCurrentShaderAndBind(SHADER_TYPE::PHONG);
    updateShader();
    qDebug()<<"Estic a Phong";

}

void GLWidget::activaGouraudShader() {
    setCurrentShaderAndBind(SHADER_TYPE::GOURAND);
    updateShader();
    qDebug()<<"Estic a Gouraud";
}

void GLWidget::activaPhongTex() {
    setCurrentShaderAndBind(SHADER_TYPE::TEXTURE);
    updateShaderTexture();
    qDebug()<<"Estic a Phong Tex";
}

void GLWidget::activaBackground() {
    if (current_behaviour == PAINT_GL_BEHAVIOUR::BACKGROUND){
        current_behaviour = previous_behaviour;
    }else{
        previous_behaviour = current_behaviour;
        current_behaviour = PAINT_GL_BEHAVIOUR::BACKGROUND;
    }
    updateGL();
    qDebug()<<"Estic a Background";
}

void GLWidget::activaDadesMultipleShading(){
    previous_behaviour = current_behaviour;
    current_behaviour=PAINT_GL_BEHAVIOUR::DIFFERENT_SHADING;
    updateGL();
}

void GLWidget::activaBGReflection() {
    setCurrentShaderAndBind(SHADER_TYPE::BG_REFLECTION);
    updateShader();
    qDebug()<<"Estic a Background Reflection";
}

void GLWidget::activaBGTransparency() {
    setCurrentShaderAndBind(SHADER_TYPE::BG_REFRACTION);
    updateShader();
    qDebug()<<"Estic a Background Refraction";
}

void GLWidget::newData(Scene *sc){
    previous_behaviour = current_behaviour;
    current_behaviour = PAINT_GL_BEHAVIOUR::DATA;
    // Control de la nova escena a la GPU

    updateScene(sc);
}

void GLWidget::newScene(Scene *sc){
    previous_behaviour = current_behaviour;
    current_behaviour = PAINT_GL_BEHAVIOUR::DEFAULT;
    // Control de la nova escena a la GPU
    updateScene(sc);
}
void GLWidget::activaBumpMapping() {
    setCurrentShaderAndBind(SHADER_TYPE::BUMP_MAPPING);
    updateShaderBump();
    qDebug()<<"Estic a Bump Mapping";
}



void GLWidget::activaEnvMapping() {
    //TO DO: a implementar a la fase 2 de la practica 2
    qDebug()<<"Estic a Environmental Mapping";
}

void GLWidget::activaTransparency() {
    //TO DO: a implementar a la fase 2 de la practica 2
    qDebug()<<"Estic a Transparencia";
}

void GLWidget::renderFrame() {
    scene->toGPU(currentProgram);
    scene->draw();
    updateGL();
}

//Metode  per canviar de shaders.
void GLWidget::updateShader(){
    scene->toGPU(currentProgram);
    updateGL();
}

//Metode per canviar de shaders de textures
void GLWidget::updateShaderTexture(){
    scene->toGPU(currentProgram);
    updateGL();
}

void GLWidget::updateShaderBump(){
    scene->toGPU(currentProgram);
    updateGL();
}
/** Mètodes que es criden des de les finestres de dialeg */

void GLWidget::setRotation(float angle, float x, float y, float z)
{
    // Rotació en coordenades de mon que es farà sobre l'escena carregada

}
void GLWidget::setTranslation(float x, float y, float z)
{
    // Translació en coordenades de mon que es farà sobre l'escena carregada

}

void GLWidget::setScale(float s)
{
    // Escala en coordenades de mon que es  farà sobre l'escena carregada
}


void GLWidget::setPerspective(float verticalAngle, float nearPlane, float farPlane)
{
    scene->camera->setFrustum(verticalAngle, nearPlane, farPlane);

    updateGL();
}

void GLWidget::setLookAt(const QVector3D &eye, const QVector3D &center, const QVector3D& up)
{
    vec4 lookfrom(eye[0], eye[1], eye[2], 1.0);
    vec4 lookat(center[0], center[1], center[2], 1.0);
    vec4 vup(up[0], up[1], up[2], 1.0);
    scene->camera->setObservador(lookfrom, lookat, vup);

    updateGL();
}

void GLWidget::setLighting(const QVector3D &lightPos, const QVector3D &Ia, const QVector3D &Id,
                           const QVector3D &Is, const QVector3D &coefs)
{
    vec3 lightPosition(lightPos[0],lightPos[1], lightPos[2]) ;
    vec3 intensityA( Ia[0], Ia[1], Ia[2]);
    vec3 intensityD( Id[0], Id[1], Id[2]);
    vec3 intensityS( Is[0], Is[1], Is[2]);

    scene->lights[0]->setIa(intensityA);
    scene->lights[0]->setId(intensityD);
    scene->lights[0]->setIs(intensityS);
    scene->lights[0]->setLightPosition(lightPosition);
    scene->lightsToGPU(currentProgram);
    updateGL();
}

void GLWidget::setTextureFile(const QString &file)
{
    QOpenGLTexture *texture;

    texture = new QOpenGLTexture(QImage(file).mirrored());

    // TO DO: A modificar en la fase 1 de la practica 2
    // Per ara es posa la textura al primer objecte de l'escena
    scene->elements[0]->setTexture(texture);
}

void GLWidget::setNormalTextureFile(const QString &file)
{
    QOpenGLTexture *texture;

    texture = new QOpenGLTexture(QImage(file).mirrored());

    // TO DO: A modificar en la fase 1 de la practica 2
    // Per ara es posa la textura al primer objecte de l'escena
    scene->elements[0]->setNormalTexture(texture);
}




void GLWidget::setBackgroundTextureFile(const QString &file)
{

}

/**  Mètodes d'interacció amb el ratolí */

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        if(lastPos.y()!= event->y() && lastPos.x()!= event->x()) {
           setXRotation(dy);
           setYRotation(dx);
        } else if(lastPos.y()!= event->y()) {// rotar la camera
            setXRotation(dy);
        } else if (lastPos.x()!= event->x()) {
            setYRotation(dx);
        }

    } else if (event->buttons() & Qt::RightButton) {
       // Zoom: canviar la mida de la window
        if(lastPos.y()> event->y())
             Zoom(-1);
        else
             Zoom(1);
    }

    lastPos = event->pos();
}


void GLWidget::setXRotation(int angle)
{
    if (angle >0) {
        scene->camera->angX += 5;
    } else if (angle<0)
        scene->camera->angX -= 5;
    qNormalizeAngle(scene->camera->angX);

    scene->camera->rotaCamera();

    emit ObsCameraChanged(scene->camera);

    updateGL();
}

void GLWidget::setYRotation(int angle)
{
    if (angle >0) {
        scene->camera->angY += 5;
    } else if (angle<0)
        scene->camera->angY-= 5;
    qNormalizeAngle(scene->camera->angY);

    scene->camera->rotaCamera();

    emit ObsCameraChanged(scene->camera);

    updateGL();
}

void GLWidget::qNormalizeAngle(double &angle)
{
    while (angle < 0)
        angle += 360*16;
    while (angle > 360)
        angle -= 360*16;
}

void GLWidget::Zoom (int positiu) {
    scene->camera->AmpliaWindow(positiu*(0.005));

    emit FrustumCameraChanged(scene->camera);

    updateGL();
}

