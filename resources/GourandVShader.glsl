#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vNormal;


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

uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 viewer;

uniform Material material;
uniform Light lights[50];
uniform int numberOfLights;
uniform vec3 lightAmbientGlobal;

out vec4 color;

void Blinn_Phong(out vec4 color);
void getLightAttenuation(in int lightPosition, out float att);

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;
    Blinn_Phong(color);
}

void Blinn_Phong(out vec4 color){

    vec3 caux = vec3(0,0,0);
    vec3 N = normalize(vec3(vNormal.x,vNormal.y,vNormal.z));
    vec3 V = normalize(vec3(viewer.x,viewer.y,viewer.z)-vec3(vPosition.x,vPosition.y,vPosition.z));
    float att;

    caux += lightAmbientGlobal*material.ambient;

    for(int i=0;i<numberOfLights;i++){
        getLightAttenuation(i,att);
        vec3 L = normalize(lights[i].position - vec3(vPosition.x,vPosition.y,vPosition.z));
        vec3 H = normalize(L+V);
        caux+= att*(lights[i].diffuse*material.diffuse*max(dot(L,N),0.0) + lights[i].specular*material.specular*pow(max(dot(N,H),0.0),material.shininess) + lights[i].ambient*material.ambient);
    }

    color = vec4(caux,1.0);
}

//coeficients.x=a0...
void getLightAttenuation(in int lightPosition, out float att){
    float d = distance(lights[lightPosition].position,vec3(vPosition.x,vPosition.y,vPosition.z));
    att = 1.0/(lights[lightPosition].coeficients.x + lights[lightPosition].coeficients.y*d + lights[lightPosition].coeficients.z*pow(d,2));
}
