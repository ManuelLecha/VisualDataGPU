#version 330

#define PUNTUAL 0
#define DIRECTIONAL 1
#define SPOT 2

#define MAX_LIGHTS 50

in vec4 color;
in vec4 pos;
in vec4 normal;
out vec4 colorOut;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 transparency;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    int lightType;
    vec3 coeficients;
    vec3 direction;
    float angle;
};

uniform Material material;
uniform Light lights[50];
uniform int numberOfLights;
uniform vec3 lightAmbientGlobal;
uniform vec4 viewer;

//Function declarations
void getColorToonInterval(out vec3 diffuseToonColor);
void emphasiseSilhouettesColor(in vec3 diffuseToonColor, out vec3 emphasizedColor);

void main()
{
    vec3 diffuseToonColor = vec3(0,0,0);
    getColorToonInterval(diffuseToonColor);
    vec3 emphasizedColor = vec3(0,0,0);
    emphasiseSilhouettesColor(diffuseToonColor, emphasizedColor);
    colorOut = vec4(diffuseToonColor,1.0);
}



void getColorToonInterval(out vec3 diffuseToonColor)
{
    float intensity = 0;
    intensity = dot(normalize(lights[0].position -  vec3(pos.x,pos.y,pos.z)), normalize(vec3(normal.x,normal.y,normal.z)));

    if(intensity > 0.95){
        diffuseToonColor = material.diffuse;
    }
    else if(intensity > 0.70){
        diffuseToonColor = 0.7*material.diffuse;
    }
    else if(intensity > 0.35){
        diffuseToonColor = 0.5*material.diffuse;
    }
    else{
        diffuseToonColor = 0.3*material.diffuse;
    }

}

void emphasiseSilhouettesColor(in vec3 diffuseToonColor, out vec3 emphasizedColor) {

    vec3 visionDirection = vec3(pos.x,pos.y,pos.z) - vec3(viewer.x, viewer.y, viewer.z);
    visionDirection = normalize(visionDirection);

    float alfa = dot(visionDirection, normalize(vec3(normal.x,normal.y,normal.z)));

    emphasizedColor = (1.0-alfa)*diffuseToonColor;
}


