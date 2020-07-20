#ifndef Object_H
#define Object_H

#include <QObject>
#include <vector>
#include "./library/Common.h"
#include "./cara.h"
#include <QGLShaderProgram>
#include <QOpenGLTexture>

#include <QFile>
#include <QRegularExpression>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <Material.h>
#include "./TG/TG.h"
#include "./animations/Animation.h"

using namespace std;

typedef Common::vec4  point4;

// Classe que conte la representacio d'un Objecte: geometria, topologia, material i textures
class Object : public QObject, public Animable {

    Q_OBJECT
protected:
    QString nom;
    vector<Cara> cares;
    vector<point4> vertexs;
    vector<point4> normalsVertexs;
    vector<vec3> tangentsVertexs;
    vector<vec3> bitangentsVertexs;
    vector<vec2>   textVertexs;
    map<int, list<int>> facesIndexsOfVertexIndex;

    // Estructures per passar a la GPU
    GLuint buffer;
    GLuint vao;

    int numPoints;
    point4 *points;
    point4 *normals;
    point4 *tangents;
    point4 *bitangents;
    vec2 *textCoordVertex;

    unsigned long Index; // index de control del numero de vertexs a passar a la GPU


    QGLShaderProgram *program;

    QOpenGLTexture *texture;
    QOpenGLTexture *background_texture;
    QOpenGLTexture *normaltexture;

public:

    Object(const int npoints, Material *material, QObject *parent = 0);
    Object(const int npoints, QString n);
    Object(QString gizmo, Material *m);
    Object(QString gizmo);
    Object();
    ~Object();

    void parseObjFile(const QString &fileName);

    void setTexture(QOpenGLTexture *t);
    void setNormalTexture(QOpenGLTexture *t);
    void setBackgroundTexture(QOpenGLTexture *t);

    virtual void make();
    virtual void toGPU(QGLShaderProgram *p);

    virtual void draw();
    virtual void drawTexture();
    virtual void drawNormalTexture();
    virtual void drawBackgroundReflection();
    virtual void drawBackgroundRefraction();

    Capsa3D calculCapsa3D();

    virtual void aplicaTG(TG *tg);

    Material *material=nullptr;


private:
    void initTexture();
    void initBackgroundTexture();
    void initNormalTexture();
    bool hasTextureVertexs();
    point4 getNormalAtVertex(int vertexIndex);
    bool createdWithFile;
    point4 centroid;
    void computeAndSetCentroid();
    void calculateTangents();
};



#endif // Object_H
