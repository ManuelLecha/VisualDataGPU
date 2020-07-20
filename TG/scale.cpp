#include "scale.h"

ScaleTG::ScaleTG(vec3 scale)
{
    this->matTG = Scale(scale);
}


ScaleTG::ScaleTG(vec3 scale, vec3 origin)
{
    this->matTG = Translate(origin)*Scale(scale)*Translate(-origin);
}
