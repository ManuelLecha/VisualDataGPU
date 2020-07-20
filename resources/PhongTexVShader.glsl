#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vNormal;
layout (location = 2) in vec2 vCoordTexture;

uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 viewer;


out vec4 normal;
out vec4 pos;
out vec2 v_texcoord;
out vec3 originalVertexPosition;

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;
    normal = vNormal;
    pos = vPosition;
    v_texcoord = vCoordTexture;
    originalVertexPosition = vec3(vPosition.x, vPosition.y, vPosition.z);
}
