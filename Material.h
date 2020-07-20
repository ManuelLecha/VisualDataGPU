#ifndef MATERIAL_H
#define MATERIAL_H

#include <QGLShaderProgram>

#include <library/vec.h>

using namespace Common;

class Material {


public:
    Material();
    Material(vec3 ambient, vec3 diffuse, vec3 specular, float sh, vec3 transparency);
    ~Material();

    void toGPU(QGLShaderProgram *program);


    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    vec3 transparency;
    vec3 transparencyComplementary;
    float shininess;

protected:
    vec3 RandomInSphere() const;
};

#endif // MATERIAL_H
