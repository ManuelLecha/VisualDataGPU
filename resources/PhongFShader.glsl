#version 330

#define PUNTUAL 0
#define DIRECTIONAL 1
#define SPOT 2

#define MAX_LIGHTS 50

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
    vec3 coeficients;
    int lightType;
    vec3 direction;
    float angle;
};

uniform Material material;
uniform Light lights[50];
uniform int numberOfLights;
uniform vec3 lightAmbientGlobal;
uniform vec4 viewer;

void Blinn_Phong(out vec4 color);
void getLightAttenuation(in int lightPosition, out float att);

void main()
{
   Blinn_Phong(colorOut);
}


void Blinn_Phong(out vec4 colorOut){

    vec3 caux = vec3(0,0,0);
    vec3 N = normalize(vec3(normal.x,normal.y,normal.z));
    vec3 V = normalize(vec3(viewer.x,viewer.y,viewer.z)-vec3(pos.x,pos.y,pos.z));
    float att;

    caux += lightAmbientGlobal*material.ambient;

    for(int i=0;i<numberOfLights;i++){
        getLightAttenuation(i,att);
        vec3 L = normalize(lights[i].position - vec3(pos.x,pos.y,pos.z));
        vec3 H = normalize(L+V);
        caux+= att*(lights[i].diffuse*material.diffuse*max(dot(L,N),0.0) + lights[i].specular*material.specular*pow(max(dot(N,H),0.0),material.shininess) + lights[i].ambient*material.ambient);
    }

    colorOut = vec4(caux,1.0);
}

//coeficients.x=a0...
void getLightAttenuation(in int lightPosition, out float att){
    float d = distance(lights[lightPosition].position,vec3(pos.x,pos.y,pos.z));
    att = 1.0/(lights[lightPosition].coeficients.x + lights[lightPosition].coeficients.y*d + lights[lightPosition].coeficients.z*pow(d,2));
}



