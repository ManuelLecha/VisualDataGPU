#ifndef PARALLELELIPED_H
#define PARALLELELIPED_H

#include <QObject>
#include <vector>
#include "./library/Common.h"
#include <QGLShaderProgram>
#include <QOpenGLTexture>

#include <QFile>
#include <QDebug>

using namespace std;
typedef Common::vec4  point4;

const int NumVertices = 36;

class CubBackground
{
protected:
    // Estructures per passar a la GPU
    GLuint buffer;
    GLuint vao;
public:
    CubBackground();
    CubBackground(vector<point4> vertexs);
    ~CubBackground();
    void setTexture(QString faces[]);
    QOpenGLTexture* getTexture();
    void toGPU(QGLShaderProgram *program);
    void draw();
    void make();
    QOpenGLTexture *texture;

private:
    QGLShaderProgram *program;
    vector<point4> vertexs;
    vector<QString> faces;
    point4 points[NumVertices]; // 36 punts: cub triangulat
    vec2 vertexsTextura[NumVertices]; // coordenades de textura associades a cada vertex
    int Index; // index de control del numero de vertexs a passar a la GPU

    void initTexture();
    void quad( int a, int b, int c, int d );
};

#endif // PARALLELELIPED_H
