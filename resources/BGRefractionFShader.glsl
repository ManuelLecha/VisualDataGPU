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
uniform samplerCube texEnvironment;

void getBackgroundColor(in vec3 dir, out vec3 color);
void Blinn_Phong_Reflexion(out vec4 color, in vec3 backgroundColor, in float weight);
void getLightAttenuation(in int lightPosition, out float att);

void main()
{
    vec3 envColor;
    getBackgroundColor(normal.xyz, envColor);
    colorOut = vec4(envColor,1);
    //Blinn_Phong_Reflexion(colorOut, envColor, 0.9);
}

void getBackgroundColor(in vec3 normal, out vec3 color){
    vec3 R = normalize(refract(normalize(pos.xyz-viewer.xyz), normalize(normal),0.9));
    color = texture(texEnvironment, R).rgb;
}

void Blinn_Phong_Reflexion(out vec4 colorOut, in vec3 backgroundColor, in float weight){

    vec3 caux = vec3(0,0,0);
    vec3 N = normalize(vec3(normal.x,normal.y,normal.z));
    vec3 V = normalize(vec3(viewer.x,viewer.y,viewer.z)-vec3(pos.x,pos.y,pos.z));
    float att;

    caux += lightAmbientGlobal*material.ambient;

    vec3 diffuseColor = backgroundColor*weight + material.diffuse*(1-weight);
    vec3 ambientColor = backgroundColor*weight + material.ambient*(1-weight);

    for(int i=0;i<numberOfLights;i++){
        getLightAttenuation(i,att);
        vec3 L = normalize(lights[i].position - vec3(pos.x,pos.y,pos.z));
        vec3 H = normalize(L+V);
        caux+= att*(lights[i].diffuse*diffuseColor*max(dot(L,N),0.0) + lights[i].specular*material.specular*pow(max(dot(N,H),0.0),material.shininess) + lights[i].ambient*ambientColor);
    }

    colorOut = vec4(caux,1.0);
}

//coeficients.x=a0...
void getLightAttenuation(in int lightPosition, out float att){
    float d = distance(lights[lightPosition].position,vec3(pos.x,pos.y,pos.z));
    att = 1.0/(lights[lightPosition].coeficients.x + lights[lightPosition].coeficients.y*d + lights[lightPosition].coeficients.z*pow(d,2));
}
