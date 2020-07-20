#ifndef SCENE_H
#define SCENE_H

#include <cmath>
#include <iostream>
#include <stdlib.h>

#include <list>
#include <vector>
#include <string>
#include <stdio.h>

#include "./library/Common.h"
#include "./objects/Object.h"
#include <Light.h>
#include <Camera.h>
#include "./objects/groundplane.h"

#include <objects/CubBackground.h>

using namespace std;

// Scene: contains all objects, lights to visualize the scene
// the viewer is placed in the z+ axis

class Scene {

public:

    vector<Object*> elements;
    vector<Light*>  lights;

    vec3    lightAmbientGlobal;
    Camera *camera;
    // Capsa contenedora de l'escena
    Capsa3D capsaMinima;

    CubBackground *capsaBackground = nullptr;

    GroundPlane *ground = nullptr;

    Scene();
    ~Scene();

    void   addObject(Object *obj);
    void   addLight(Light *l);
    void   setLightGlobalAmbient(vec3 lightGlobalAmbient);
    Light *getLightActual();
    void   setLightActual(Light* l);

    void setCamera(Camera *c);

    void toGPU(QGLShaderProgram *p);
    void draw();
    void drawTexture();
    void drawNormalTexture();
    void drawBackgroundReflection();
    void drawBackgroundRefraction();

    void setBackgroundTexture(QString texture[]);
    void lightsToGPU(QGLShaderProgram *program);
    void setAmbientToGPU(QGLShaderProgram *program);

    void calculCapsaMinCont3DEscena();
    void updateCapsaBackgroundBoundaries();
    void update(int nframe);

    void renderBackground(QGLShaderProgram *program);
    void renderPlane(QGLShaderProgram *program);

};

#endif // SCENE_H
