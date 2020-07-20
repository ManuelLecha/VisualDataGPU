#include "Translation.h"

Translation::Translation(vec3 trasl): traslation(trasl)
{
    matTG[0][3] = trasl.x;
    matTG[1][3] = trasl.y;
    matTG[2][3] = trasl.z;
}

Translation::~Translation()
{

}

