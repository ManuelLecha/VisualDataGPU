#include "Toonmaterial.h"
#include <QOpenGLFunctions>


ToonMaterial::ToonMaterial() {
    transparency = vec3(0.0, 0.0, 0.0);
    transparencyComplementary = vec3(1.0, 1.0, 1.0);
    diffuse1 = vec3(1.0, 1.0, 1.0);
    diffuse2 = vec3(0.8, 0.8, 0.8);
    diffuse3 = vec3(0.6, 0.6, 0.6);
    diffuse4 = vec3(0.4, 0.4, 0.4);
}

ToonMaterial::ToonMaterial(vec3 ambient,
                   vec3 diffuse1,
                   vec3 diffuse2,
                   vec3 diffuse3,
                   vec3 diffuse4,
                   vec3 specular,
                   float sh,
                   vec3 transparency)
{

    this->diffuse1 = diffuse1;
    this->diffuse1 = diffuse2;
    this->diffuse1 = diffuse3;
    this->diffuse1 = diffuse4;
    this->ambient = ambient;
    this->specular = specular;
    this->shininess = sh;
    this->transparency = transparency;
    this->transparencyComplementary = vec3(1.0, 1.0, 1.0) - transparency;
}


ToonMaterial::~ToonMaterial() {}

/**
 * Passa el material de CPU a GPU
 * @brief ToonMaterial::toGPU
 * @param program
 */
void ToonMaterial::toGPU(QGLShaderProgram *program){

    program->setUniformValue("material.ambient", this->ambient.x, this->ambient.y, this->ambient.z);
    program->setUniformValue("material.diffuse1", this->diffuse1.x, this->diffuse1.y, this->diffuse1.z);
    program->setUniformValue("material.diffuse2", this->diffuse2.x, this->diffuse2.y, this->diffuse2.z);
    program->setUniformValue("material.diffuse3", this->diffuse3.x, this->diffuse3.y, this->diffuse3.z);
    program->setUniformValue("material.diffuse4", this->diffuse4.x, this->diffuse4.y, this->diffuse4.z);
    program->setUniformValue("material.specular", this->specular.x, this->specular.y, this->specular.z);
    program->setUniformValue("material.transparency", this->transparency.x, this->transparency.y, this->transparency.z);
    program->setUniformValue("material.shininess", this->shininess);
}
