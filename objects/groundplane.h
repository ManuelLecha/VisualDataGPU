#ifndef GROUNDPLANE_H
#define GROUNDPLANE_H
#include <./library/Common.h>
#include <Material.h>
#include <QGLShaderProgram>
#include <QOpenGLTexture>

typedef vec4  color4;
typedef vec4  point4;

//const int NumVertices = 6;

#define GROUND_MATERIAL 0
#define GROUND_TEXTURE 1

class GroundPlane
{
protected:
    // Estructures per passar a la GPU
    GLuint buffer;
    GLuint vao;
public:
    GroundPlane(float width, float height, QString textureFile);
    GroundPlane();
    void make();
    void toGPU(QGLShaderProgram* program);
    void draw();

    QString textureFile = nullptr;
    Material* m = nullptr;

private:
    void initTextura();
    void quad( int a, int b, int c, int d );
    int Index;
    QGLShaderProgram *program;

    point4 vertices[4];
    color4 vertex_colors[4];

    point4 points[6]; // 6 punts: cub triangulat
    vec2 vertexsTextura[6]; // coordenades de textura associades a cada vertex

    QOpenGLTexture *texture;

};

#endif // GROUNDPLANE_H
