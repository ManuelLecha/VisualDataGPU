#include "Scene.h"
#include <iostream>
#include <string>
#include <QString>

/**
 * @brief Scene::Scene
 */
Scene::Scene() {
    // Inicialització de la capsa minima
    capsaMinima.pmin = vec3(-1.0, -1.0,-1.0);
    capsaMinima.a = 2;
    capsaMinima.h = 2;
    capsaMinima.p = 2;
    updateCapsaBackgroundBoundaries();
}

/**
 * @brief Scene::~Scene
 */
Scene::~Scene() {
    elements.clear();
    lights.clear();
    delete camera;
    delete capsaBackground;
}

/**
 * @brief Scene::addObject
 * @param obj
 */
void Scene::addObject(Object *obj) {
    elements.push_back(obj);
    obj->setBackgroundTexture(capsaBackground->texture);
    calculCapsaMinCont3DEscena();
}

/**
 * @brief Scene::toGPU
 */
void Scene::toGPU(QGLShaderProgram *p) {
    this->lightsToGPU(p);
    this->setAmbientToGPU(p);
    this->camera->toGPU(p);
    for(unsigned int i=0; i < elements.size(); i++){
        elements.at(i)->toGPU(p);
    }
}

/**
 * @brief Scene::draw
 */
void Scene::draw() {
    for(unsigned int i=0; i < elements.size(); i++){
        elements.at(i)->draw();
    }
}

/**
 * @brief Scene::drawTexture
 */
void Scene::drawTexture() {
    for(unsigned int i=0; i < elements.size(); i++){
        elements.at(i)->drawTexture();
    }
}


void Scene::drawNormalTexture() {
    for(unsigned int i=0; i < elements.size(); i++){
        elements.at(i)->drawNormalTexture();
    }
}

void Scene::drawBackgroundReflection() {
    for(unsigned int i=0; i < elements.size(); i++){
        elements.at(i)->drawBackgroundReflection();
    }
}

void Scene::drawBackgroundRefraction(){
    for(unsigned int i=0; i < elements.size(); i++){
        elements.at(i)->drawBackgroundRefraction();
    }
}

void Scene::setBackgroundTexture(QString texture[])
{
    this->capsaBackground->setTexture(texture);
}

void Scene::renderBackground(QGLShaderProgram *program) {
    //this->capsaBackground->make();
    this->capsaBackground->toGPU(program);
    this->capsaBackground->draw();
}

void Scene::renderPlane(QGLShaderProgram *program){
    if (this->ground != nullptr){
        qDebug()<<"renderPlane";
        this->ground->toGPU(program);
        this->ground->draw();
    }
}

/**
 * @brief Scene::getLightActual
 * @return
 */
Light* Scene::getLightActual() {
    // TO DO OPCIONAL: A modificar si es vol canviar el comportament de la GUI
    // Ara per ara dona com a Light actual la darrera que s'ha inserit
    return (lights[lights.size()-1]);
}

/**
 * @brief Scene::setLightActual
 * @param l
 */
void Scene::setLightActual(Light* l){
    lights[lights.size()-1]=l;
}



/**
 * @brief Scene::lightsToGPU
 * @param program
 */
void Scene::lightsToGPU(QGLShaderProgram *program){
    int numberOfLights = lights.size();
    program->setUniformValue("numberOfLights", numberOfLights);

    Light *light;
    for(int i=0; i < numberOfLights; i++) {

        light = lights.at(i);

        vec3 ambient = light->getIa();
        std::string ambientIndexor = "lights["+std::to_string(i)+"].ambient";
        program->setUniformValue(ambientIndexor.c_str(), ambient.x, ambient.y, ambient.z);

        vec3 diffuse = light->getId();
        std::string diffuseIndexor = "lights["+std::to_string(i)+"].diffuse";
        program->setUniformValue(diffuseIndexor.c_str(),diffuse.x, diffuse.y,diffuse.z);

        vec3 specular = light->getIs();
        std::string specularIndexor = "lights["+std::to_string(i)+"].specular";
        program->setUniformValue(specularIndexor.c_str(),specular.x, specular.y,specular.z);

        vec3 position = light->getLightPosition();
        std::string positionIndexor = "lights["+std::to_string(i)+"].position";
        program->setUniformValue(positionIndexor.c_str(),position.x, position.y,position.z);

        vec3 coeficients = light->getCoeficients();
        std::string coeficientsIndexor = "lights["+std::to_string(i)+"].coeficients";
        program->setUniformValue(coeficientsIndexor.c_str(),coeficients.x, coeficients.y,coeficients.z);

        std::string lightTypeIndexor = "lights["+std::to_string(i)+"].lightType";
        program->setUniformValue(lightTypeIndexor.c_str(),light->getTipusLight());

        vec3 direction = light->getDirection();
        std::string directionIndexor = "lights["+std::to_string(i)+"].direction";
        program->setUniformValue(directionIndexor.c_str(),direction.x, direction.y,direction.z);

        std::string angleIndexor = "lights["+std::to_string(i)+"].angle";
        program->setUniformValue(angleIndexor.c_str(),light->getAngle());
    }
}

void Scene::addLight(Light *l) {
    lights.push_back(l);
}


void Scene::setLightGlobalAmbient(vec3 lightAmbientGlobal){
    this->lightAmbientGlobal=lightAmbientGlobal;
}

/**
 * @brief Scene::setAmbientToGPU
 * @param program
 */
void Scene::setAmbientToGPU(QGLShaderProgram *program){
    // TO DO: A implementar a la fase 1 de la practica 2
    program->setUniformValue("lightAmbientGlobal", this->lightAmbientGlobal.x, this->lightAmbientGlobal.y, this->lightAmbientGlobal.z);
}

/**
 * @brief Scene::setCamera
 * @param Camera *
 */
void Scene::setCamera(Camera *c) {
    camera = c;
}
/**
 * @brief Scene::calculCapsaMinCont3DEscena
 */

void Scene::calculCapsaMinCont3DEscena()
{
    Capsa3D c;
    vec3 pmax;

    if (elements.size()==1) {
        capsaMinima = elements[0]->calculCapsa3D();
        pmax[0] = capsaMinima.pmin[0]+capsaMinima.a;
        pmax[1] = capsaMinima.pmin[1]+capsaMinima.h;
        pmax[2] = capsaMinima.pmin[2]+capsaMinima.p;
    } else {
        capsaMinima.pmin[0]=200;
        capsaMinima.pmin[1]=200;
        capsaMinima.pmin[2]=200;
        pmax[0] = -200;
        pmax[1] = -200;
        pmax[2] = -200;
    }

    for (unsigned int i=0; i<elements.size(); i++) {
       c = elements[i]->calculCapsa3D();

       if (capsaMinima.pmin[0]>c.pmin[0]) capsaMinima.pmin[0] = c.pmin[0];
       if (capsaMinima.pmin[1]>c.pmin[1]) capsaMinima.pmin[1] = c.pmin[1];
       if (capsaMinima.pmin[2]>c.pmin[2]) capsaMinima.pmin[2] = c.pmin[2];
       if (pmax[0]<c.pmin[0]+c.a) pmax[0] = c.pmin[0]+c.a;
       if (pmax[1]<c.pmin[1]+c.h) pmax[1] = c.pmin[1]+c.h;
       if (pmax[2]<c.pmin[2]+c.p) pmax[2] = c.pmin[2]+c.p;
    }
    capsaMinima.a = pmax[0]-capsaMinima.pmin[0];
    capsaMinima.h = pmax[1]-capsaMinima.pmin[1];
    capsaMinima.p = pmax[2]-capsaMinima.pmin[2];
}

void Scene::updateCapsaBackgroundBoundaries() {
    vec3 magnification = vec3(15.0, 15.0, 15.0);
    //vec3 pmin = capsaMinima.pmin - magnification;
    //vec3 pmax = vec3(pmin.x + capsaMinima.a, pmin.y + capsaMinima.h, pmin.z + capsaMinima.p) + magnification;

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
    //CubBackground * parallelepiped = new CubBackground();
    if(capsaBackground != nullptr) {
        delete capsaBackground;
    }
    capsaBackground = parallelepiped;
}


void Scene::update(int nframe) {
    for (unsigned int i = 0; i< elements.size(); i++) {
        if (dynamic_cast<Animable*>(elements[i]) ) elements[i]->update(nframe);
    }
}
