#ifndef SCALE_H
#define SCALE_H

#include "./TG/TG.h"


class ScaleTG : public TG
{
public:
    ScaleTG(vec3 scale);
    ScaleTG(vec3 scale, vec3 origin);
};

#endif // SCALE_H
