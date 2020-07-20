#version 330

#define PUNTUAL 0
#define DIRECTIONAL 1
#define SPOT 2

#define MAX_LIGHTS 50
#define TEXTURE_PERCENTAGE 75
#define M_PI 3.1415926535897932384626433832795

in vec4 pos;
in vec4 normal;
in vec4 tangent;
in vec4 bitangent;
in vec2 v_texcoord;
in vec3 originalVertexPosition;

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
uniform bool hasTextureVertexs;
uniform vec4 centerOfMass;

uniform sampler2D texMap;
uniform sampler2D texNormalMap;

void Blinn_Phong(out vec4 color, in vec3 diffuseText);
void getLightAttenuation(in int lightPosition, out float att);
void getTextureValue(in sampler2D tex, out vec3 textureValue);
void getTextureValueWithoutTexturePoint(in sampler2D tex, out vec3 textureValue);
void calculateNormal(out vec3 tnormal);
void printVector(in vec3 v);

void main()
{
    vec3 textureValue;
    getTextureValue(texMap, textureValue);
    float textPercentage = 1;
    vec3 mixTextureAndAmbient = textPercentage*textureValue + (1-textPercentage)*material.diffuse;
    Blinn_Phong(colorOut, mixTextureAndAmbient);
}

void calculateNormal(out vec3 tnormal){
    vec3 mapValue;
    getTextureValue(texNormalMap, mapValue);
    vec3 mapNormal = (mapValue)*2-1;
    //mapNormal = normalize(vec3(0,-1,1));
    tnormal = normalize(tangent.xyz)*mapNormal.x + normalize(bitangent.xyz)*mapNormal.y + normalize(normal.xyz)*mapNormal.z;
    float weight = 1;
    tnormal = (normal.xyz*(1-weight)+ weight*tnormal);
    //printVector(mapNormal);
}


void printVector(in vec3 v){
    colorOut = vec4(v,1.0f);
}


void Blinn_Phong(out vec4 colorOut, in vec3 diffuseText){
    vec3 nnormal;
    calculateNormal(nnormal);
    vec3 caux = vec3(0,0,0);
    vec3 N = normalize(vec3(nnormal.x,nnormal.y,nnormal.z));
    vec3 V = normalize(vec3(viewer.x,viewer.y,viewer.z)-vec3(pos.x,pos.y,pos.z));
    float att;

    caux += lightAmbientGlobal*material.ambient;

    for(int i=0;i<numberOfLights;i++){
        getLightAttenuation(i,att);
        vec3 L = normalize(lights[i].position - vec3(pos.x,pos.y,pos.z));
        vec3 H = normalize(L+V);
        caux+= att*(lights[i].diffuse*diffuseText*max(dot(L,N),0.0) + lights[i].specular*material.specular*pow(max(dot(N,H),0.0),material.shininess) + lights[i].ambient*diffuseText);
        //caux+= att*(lights[i].diffuse*diffuseText*max(dot(L,N),0.0) + lights[i].ambient*material.ambient);
    }

    colorOut = vec4(caux,1.0);
}

//coeficients.x=a0...
void getLightAttenuation(in int lightPosition, out float att){
    float d = distance(lights[lightPosition].position,vec3(pos.x,pos.y,pos.z));
    att = 1.0/(lights[lightPosition].coeficients.x + lights[lightPosition].coeficients.y*d + lights[lightPosition].coeficients.z*pow(d,2));
}

void getTextureValue(in sampler2D tex, out vec3 textureValue) {
    if(hasTextureVertexs) {
        textureValue  = vec3(texture(tex, v_texcoord).rgb);
    }
    else {
        getTextureValueWithoutTexturePoint(tex, textureValue);
    }
}

void getTextureValueWithoutTexturePoint(in sampler2D tex, out vec3 textureValue) {
    vec3 normalizedVector = normalize(vec3(originalVertexPosition.x-centerOfMass.x, originalVertexPosition.y-centerOfMass.y, originalVertexPosition.z-centerOfMass.z));
    float u = 0.5 - atan(normalizedVector.z, normalizedVector.x)/(2*M_PI);
    float v = asin(normalizedVector.y)/(M_PI) - 0.5;
    textureValue  = vec3(texture2D(tex, vec2(u, -v)).rgb);
}
