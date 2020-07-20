#ifndef PLANE_H
#define PLANE_H

#include "cara.h"
#include "objects/Object.h"

class Plane : public Object{

public:
    Plane(vec3 center, vec3 up, vec3 right);
public:
    vec3 center;
    vec3 up;
    vec3 right;
};

#endif // PLANE_H
