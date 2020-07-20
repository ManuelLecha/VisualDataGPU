#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vCoordTexture;

uniform mat4 model_view;
uniform mat4 projection;

out vec2 v_texcoord;

out vec4 colorBORRAR;

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;
    v_texcoord = vCoordTexture;
    colorBORRAR = vPosition;
}
