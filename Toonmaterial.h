#ifndef TOONMATERIAL_H
#define TOONMATERIAL_H

#include <QGLShaderProgram>
#include <library/vec.h>


using namespace Common;
#include "Material.h"


class ToonMaterial: public Material{

public:
    ToonMaterial();
    ToonMaterial(vec3 ambient, vec3 diffuse1,vec3 diffuse2, vec3 diffuse3, vec3 diffuse4 , vec3 specular, float sh, vec3 transparency);
    ~ToonMaterial();

    void toGPU(QGLShaderProgram *program);

    vec3 diffuse1;
    vec3 diffuse2;
    vec3 diffuse3;
    vec3 diffuse4;
    vec3 ambient;
    vec3 specular;
    vec3 transparency;
    vec3 transparencyComplementary;
    float shininess;

protected:
    //vec3 RandomInSphere() const;

};

#endif // TOONMATERIAL_H
