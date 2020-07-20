#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "TG.h"

class Translation : public TG
{
public:
    vec3 traslation;
    Translation(vec3 trasl);
    ~Translation();
};

#endif // TRANSLATE_H
