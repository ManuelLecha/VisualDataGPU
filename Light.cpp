#include "Light.h"

/**
 * @brief Light::Light
 * @param t
 */
Light::Light(LightType t) {
    this->lightType = t;

}

/**
 * @brief Light::getDiffuseIntensity
 * @return
 */
vec3 Light::getId() {
   return this->diffuse;
}

/**
 * @brief Light::setId
 * @param i
 */
void Light::setId(vec3 i) {
    this->diffuse = i;

}

/**
 * @brief Light::getLightPosition
 */
vec3 Light::getLightPosition() {
    return this->position;
}

/**
 * @brief Light::setLightPosition
 * @param v
 */
void Light::setLightPosition(vec3 position) {
    this->position = position;
}


/**
 * @brief Light::getIa
 * @return
 */
vec3 Light::getIa() const
{
    return this->ambient;
}

/**
 * @brief Light::setIa
 * @param value
 */
void Light::setIa(const vec3 &value)
{
    this->ambient = value;

}

/**
 * @brief Light::getIs
 * @return
 */
vec3 Light::getIs() const
{
    return this->specular;
}

/**
 * @brief Light::setIs
 * @param value
 */
void Light::setIs(const vec3 &value)
{
    this->specular = value;
}

/**
 * @brief Light::getCoeficients
 * @return
 */
vec3 Light::getCoeficients() const
{
    return this->coefficients;
}

/**
 * @brief Light::setCoeficients
 * @param value
 */
void Light::setCoeficients(const vec3 &value)
{
    this->coefficients = value;
}


/**
 * @brief Light::getTipusLight
 *
 */
LightType Light::getTipusLight() const
{
    return this->lightType;
}

/**
 * @brief Light::setTipusLight
 * @param value
 */
void Light::setTipusLight(const LightType &value)
{
    this->lightType = value;
}

/**
 * @brief Light::getDirection
 * @return
 */
vec3 Light::getDirection() const {
    return this->direction;
}

/**
 * @brief Light::setDirection
 * @param dir
 */
void Light::setDirection(const vec3 dir) {
    this->direction = dir;
}

/**
 * @brief Light::getAngle
 * @return
 */
float Light::getAngle() const {
    return this->angle;
}

/**
 * @brief Light::setAngle
 * @param angle
 */
void Light::setAngle(const float angle) {
    this->angle = angle;
}
