#include "Material.h"
#include <QOpenGLFunctions>


Material::Material() {
    transparency = vec3(0.0, 0.0, 0.0);
    transparencyComplementary = vec3(1.0, 1.0, 1.0);
    diffuse = vec3(1.0, 1.0, 1.0);
}

Material::Material(vec3 ambient,
                   vec3 diffuse,
                   vec3 specular,
                   float sh,
                   vec3 transparency)
{

    this->diffuse = diffuse;
    this->ambient = ambient;
    this->specular = specular;
    this->shininess = sh;
    this->transparency = transparency;
    this->transparencyComplementary = vec3(1.0, 1.0, 1.0) - transparency;
}

/*
vec3 Material::getDiffuse(vec2 point) const {
    return this->diffuse;
}

bool Material::scatter(const Ray &r_in, const IntersectionInfo &rec, std::vector<vec3> &colors,
                       std::vector<Ray> &r_out) const {
    return false;
}
*/

vec3 Material::RandomInSphere() const {
    vec3 p;
    do {
        p = 2.0f * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (length(p) >= 1.0f);
    return p;
}

Material::~Material() {}

/**
 * Passa el material de CPU a GPU
 * @brief Material::toGPU
 * @param program
 */
void Material::toGPU(QGLShaderProgram *program){

    program->setUniformValue("material.ambient", this->ambient.x, this->ambient.y, this->ambient.z);
    program->setUniformValue("material.diffuse", this->diffuse.x, this->diffuse.y, this->diffuse.z);
    program->setUniformValue("material.specular", this->specular.x, this->specular.y, this->specular.z);
    program->setUniformValue("material.transparency", this->transparency.x, this->transparency.y, this->transparency.z);
    program->setUniformValue("material.shininess", this->shininess);
}
