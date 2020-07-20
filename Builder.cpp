#include "Builder.h"

#include <readers/datareader.h>
#include "colormaps/ColorMapStatic.h"
#include <string>
#include <readers/scenereader.h>

Builder::Builder(GLWidget *glWid)
{
    glWidget = glWid;
    scene = glWidget->getScene();

}

void Builder::newObjFromFile()
{
    QString fileName = QFileDialog::getOpenFileName();
    if (!fileName.isNull()) {
            Object * obj = new Object(100000, fileName); //todo read
            if (obj->material == nullptr){
                obj->material=new Material();
            }
            scene->addObject(obj);
            scene->camera->actualitzaCamera(scene->capsaMinima);
            emit newObj(obj);
    }
}

void Builder::newVirtualScene() {
    // TO DO Fase 1: creació de la nova escena Virtual a partir d'un fitxer VIRTUAL
    QString fileName = QFileDialog::getOpenFileName();

    scenereader sreader = scenereader(scene);
    sreader.readFile(fileName);

    emit newScene(scene);
}
void Builder::newVirtualSceneUI(){
    std::cout << "estic a newVirtualSceneUI" <<endl;
}

void Builder::newDataSceneUI(QString filename, QString gizmo, QString texture, QString colormap) {
    std::cout << "estic a scene dades" <<endl;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Error opening the file" << std::endl;
        return;
    }
    QString allText = file.readAll();
    file.close();
}


void Builder::newDataScene(QString data, QString gizmo, QString texture, QString colormap,
                      double xmin, double xmax,
                      double zmin, double zmax)
{
    // TO DO Fase 1: Crear la nova escena a partir de les dades
    // de la finestra de visualization mapping corresponent
    DataReader datareader = DataReader(scene, xmax, xmin, zmax, zmin, 0, 1, texture);
    datareader.gizmo = gizmo;

    ColorMapStatic::ColorMapType t = ColorMapStatic::ColorMapType::COLOR_MAP_TYPE_INFERNO;

    if (colormap.compare("inferno")==0){
        t=ColorMapStatic::ColorMapType::COLOR_MAP_TYPE_INFERNO;
    }else if(colormap.compare("jet")==0){
        t=ColorMapStatic::ColorMapType::COLOR_MAP_TYPE_JET;
    }else if(colormap.compare("magma")==0){
        t=ColorMapStatic::ColorMapType::COLOR_MAP_TYPE_MAGMA;
    }

    datareader.cmap = ColorMapStatic(t);
    datareader.readFile(data);

    emit newData(scene);
}

void Builder::newSceneDemo1(){
    QString earthFile = "://resources/models/demo/earth.obj";
    QString moonFile = "://resources/models/demo/moon.obj";
    QString sunFile = "://resources/models/demo/sun.obj";
    QString spaceMap[] = {
        "://resources/textures/space/left.png", //negx
        "://resources/textures/space/bottom.png", //negy
        "://resources/textures/space/back.png", //negz
        "://resources/textures/space/right.png", //posx
        "://resources/textures/space/top.png", //posy
        "://resources/textures/space/front.png", //posz
    };

    Object* earth = new Object(100000,earthFile);
    Object* moon = new Object(100000, moonFile);
    Object* sun = new Object(100000, sunFile);

    vec3 earthPos = vec3(0,0,0);
    vec3 earthSize = vec3(1,1,1);
    vec3 moonPos = vec3(1, 0, 0);
    vec3 moonSize = vec3(0.5, 0.5, 0.5);
    vec3 sunPos = vec3(0,0,4);
    vec3 sunSize = vec3(3,3,3);

    mat4 earthTM = Translate(earthPos)*Scale(earthSize);
    mat4 moonTM = Translate(moonPos)*Scale(moonSize);
    mat4 sunTM = Translate(sunPos)*Scale(sunSize);
    TG earthTG;
    earthTG.matTG = earthTM;
    TG moonTG;
    moonTG.matTG = moonTM;
    TG sunTG;
    sunTG.matTG = sunTM;

    earth->aplicaTG(&earthTG);
    moon->aplicaTG(&moonTG);
    sun->aplicaTG(&sunTG);

    scene->addObject(earth);
    scene->addObject(moon);
    scene->addObject(sun);

    vec3 pmin = vec3(-4,-4,-4);
    vec3 pmax = vec3(4,4,4);

    vector<point4> vertexs;

    vertexs.push_back(point4(pmin.x, pmin.y, pmin.z, 1.0));
    vertexs.push_back(point4(pmax.x, pmin.y, pmin.z, 1.0));
    vertexs.push_back(point4(pmax.x, pmax.y, pmin.z, 1.0));
    vertexs.push_back(point4(pmin.x, pmax.y, pmin.z, 1.0));

    vertexs.push_back(point4(pmin.x, pmin.y, pmax.z, 1.0));
    vertexs.push_back(point4(pmax.x, pmin.y, pmax.z, 1.0));
    vertexs.push_back(point4(pmax.x, pmax.y, pmax.z, 1.0));
    vertexs.push_back(point4(pmin.x, pmax.y, pmax.z, 1.0));

    CubBackground * parallelepiped = new CubBackground(vertexs);


    scene->capsaBackground = parallelepiped;

    scene->setBackgroundTexture(spaceMap);
    scene->capsaBackground->make();

    vec3 lightPos = vec3(0,0,1);
    scene->lights[0]->setLightPosition(lightPos);

    emit demo1(scene);
}

void Builder::newSceneDemo2(){
    Object* sphere = new Object(50000, "://resources/models/sphere0.obj");
    scene->addObject(sphere);
    emit demo2(scene);
}


