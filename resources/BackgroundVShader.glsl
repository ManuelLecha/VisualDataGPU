#version 330

layout (location = 0) in vec4 vPosition;

out vec4 position;
out vec3 v_texcoord; //textura

uniform mat4 model_view;
uniform mat4 projection;

void main(void)
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;
    //gl_Position = vPosition;
    position = vPosition;
    //Pas de les coordenades de textura al fragment shader
    //el valor del color i les coordenades de textura s'interpolaran automaticament
    //en els fragments interiors a les cares dels poligons
    v_texcoord = normalize(position.xyz);//textura
}
