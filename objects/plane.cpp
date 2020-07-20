#include "plane.h"

Plane::Plane(vec3 center, vec3 up, vec3 right){
    this->center=center;
    this->up=up;
    this->right=right;
    numPoints = 6;
    points = new point4[6];
    normals = new point4[6];
    textCoordVertex = new vec2[6];
    vec3 ur = up + right;
    vec3 ul = up - right;
    vec3 dr = -up+right;
    vec3 dl = -up-right;

    vec3 normal = cross(up, right);

    vertexs.push_back(vec4(ur.x, ur.y, ur.z, 1.0f));
    vertexs.push_back(vec4(ul.x, ul.y, ul.z, 1.0f));
    vertexs.push_back(vec4(dr.x, dr.y, dr.z, 1.0f));
    vertexs.push_back(vec4(dl.x, dl.y, dl.z, 1.0f));

    normalsVertexs.push_back(vec4(normal.x, normal.y, normal.z, 0.0f));
    normalsVertexs.push_back(vec4(normal.x, normal.y, normal.z, 0.0f));
    normalsVertexs.push_back(vec4(normal.x, normal.y, normal.z, 0.0f));
    normalsVertexs.push_back(vec4(normal.x, normal.y, normal.z, 0.0f));

    Cara c1, c2;
    c1.idxVertices.push_back(0);
    c1.idxVertices.push_back(3);
    c1.idxVertices.push_back(2);
    c2.idxVertices.push_back(0);
    c2.idxVertices.push_back(1);
    c2.idxVertices.push_back(3);

    cares.push_back(c1);
    cares.push_back(c2);

    textVertexs.push_back(vec2(1,1));
    textVertexs.push_back(vec2(0,1));
    textVertexs.push_back(vec2(1,0));
    textVertexs.push_back(vec2(0,0));

    this->make();
    material = new Material();
}

/**
 * @brief Object::make
 */
/*void Plane::make(){

    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal calcular la normal a cada vertex a la CPU

    Index = 6;
    numPoints = 6;
    points = new point4[6];
    normals = new point4[6];
    textCoordVertex = new vec2[6];

    points[0]=center+up+right;//1,1
    points[1]=center+up-right;//0,1
    points[2]=center-up-right;//0,0
    points[3]=center-up-right;//0,0
    points[4]=center-up+right;//0,1
    points[5]=center+up+right;//1,1
    normals[0]=cross(up,-right);
    normals[1]=cross(up,-right);
    normals[2]=cross(up,-right);
    normals[3]=cross(up,-right);
    normals[4]=cross(up,-right);
    normals[5]=cross(up,-right);

    textCoordVertex[0]=vec2(1,1);
    textCoordVertex[1]=vec2(0,1);
    textCoordVertex[2]=vec2(0,0);
    textCoordVertex[3]=vec2(0,0);
    textCoordVertex[4]=vec2(0,1);
    textCoordVertex[5]=vec2(1,1);

}*/
