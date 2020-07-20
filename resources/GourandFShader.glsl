#version 330

#define PUNTUAL 0
#define DIRECTIONAL 1
#define SPOT 2

#define MAX_LIGHTS 50

in vec4 color;
out vec4 colorOut;

void main()
{
    colorOut = color;
}
