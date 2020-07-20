#include "CubBackground.h"
#include <QCoreApplication>
#include <QGLShaderProgram>


//Testing purposes
CubBackground::CubBackground()
{
    vertexs.push_back(point4( -0.5, -0.5, -0.5, 1.0 ));
    vertexs.push_back(point4(  0.5, -0.5, -0.5, 1.0 ));
    vertexs.push_back(point4(  0.5,  0.5, -0.5, 1.0 ));
    vertexs.push_back(point4(  -0.5, 0.5, -0.5, 1.0 ));

    vertexs.push_back(point4( -0.5, -0.5,  0.5, 1.0 ));
    vertexs.push_back(point4(  0.5, -0.5,  0.5, 1.0 ));
    vertexs.push_back(point4(  0.5,  0.5,  0.5, 1.0 ));
    vertexs.push_back(point4( - 0.5, 0.5,  0.5, 1.0 ));

    faces.push_back("://resources/textures/MountainCubMap/negx.jpg");
    faces.push_back("://resources/textures/MountainCubMap/negy.jpg");
    faces.push_back("://resources/textures/MountainCubMap/negz.jpg");
    faces.push_back("://resources/textures/MountainCubMap/posx.jpg");
    faces.push_back("://resources/textures/MountainCubMap/posy.jpg");
    faces.push_back("://resources/textures/MountainCubMap/posz.jpg");
}

CubBackground::CubBackground(vector<point4> vertexs)
{
    this->vertexs = vertexs;
    faces.push_back("://resources/textures/MountainCubMap/negx.jpg");
    faces.push_back("://resources/textures/MountainCubMap/negy.jpg");
    faces.push_back("://resources/textures/MountainCubMap/negz.jpg");
    faces.push_back("://resources/textures/MountainCubMap/posx.jpg");
    faces.push_back("://resources/textures/MountainCubMap/posy.jpg");
    faces.push_back("://resources/textures/MountainCubMap/posz.jpg");
}

CubBackground::~CubBackground(){
    this->vertexs.clear();
}


void CubBackground::setTexture(QString faceFiles[])
{
    faces.clear();
    faces.push_back(faceFiles[0]);
    faces.push_back(faceFiles[1]);
    faces.push_back(faceFiles[2]);
    faces.push_back(faceFiles[3]);
    faces.push_back(faceFiles[4]);
    faces.push_back(faceFiles[5]);
    initTexture();
}

QOpenGLTexture *CubBackground::getTexture()
{
    return this->texture;
}

void CubBackground::toGPU(QGLShaderProgram *program)
{
    this->program = program;
    glGenVertexArrays( 1, &vao );

    // Creacio i inicialitzacio d'un vertex buffer object (VBO)
    glGenBuffers( 1, &buffer );

    texture->bind((texture->textureId()));
    this->program->setUniformValue("texEnvironment", texture->textureId());
    glEnable(GL_TEXTURE_CUBE_MAP);
}

void CubBackground::initTexture()
{
    qDebug() << "Initializing background textures...";

    QImage image[6];
    for (GLuint i = 0; i<faces.size();i++){
        image[i] = QImage(faces[i]).convertToFormat(QImage::Format_RGBA8888);
    }

    glActiveTexture(GL_TEXTURE0);

    texture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    if (!texture->isCreated())
        texture->create();

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture->textureId());

    texture->setFormat(QOpenGLTexture::RGBAFormat);
    texture->setSize(image[0].width(), image[0].height(), image[0].depth());
    texture->generateMipMaps();
    texture->allocateStorage();

    texture->setData(0,0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (const void*)image[0].constBits(),0);
    texture->setData(0,0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (const void*)image[1].constBits(),0);
    texture->setData(0,0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (const void*)image[2].constBits(),0);
    texture->setData(0,0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (const void*)image[3].constBits(),0);
    texture->setData(0,0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (const void*)image[4].constBits(),0);
    texture->setData(0,0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (const void*)image[5].constBits(),0);


    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void CubBackground::make()
{
    Index = 0;
    //Exterior oriented faces.
    /*quad( 3, 2, 1, 0 );
    quad( 0, 1, 5, 4 );
    quad( 3, 7, 4, 0 );
    quad( 5, 4, 7, 6 );
    quad( 1, 5, 6, 2 );
    quad( 2, 6, 7, 3 );
*/

    //Interior oriented faces.

    quad( 0, 3, 7, 4 );//negx
    quad( 0, 1, 2, 3 );//negy
    quad( 0, 4, 5, 1 );//negz

    quad( 2, 1, 5, 6 );//posx
    quad( 4, 7, 6, 5 );//posy
    quad( 3, 2, 6, 7 );//posz


    initTexture();

}

void CubBackground::quad(int a, int b, int c, int d)
{
    points[Index] = vertexs[a];
    vertexsTextura[Index] = vec2(0.0, 0.0); Index++;
    points[Index] = vertexs[b];
    vertexsTextura[Index] = vec2(1.0, 0.0); Index++;
    points[Index] = vertexs[c];
    vertexsTextura[Index] = vec2(1.0, 1.0); Index++;
    points[Index] = vertexs[a];
    vertexsTextura[Index] = vec2(0.0, 0.0); Index++;
    points[Index] = vertexs[c];
    vertexsTextura[Index] = vec2(1.0, 1.0); Index++;
    points[Index] = vertexs[d];
    vertexsTextura[Index] = vec2(0.0, 1.0); Index++;

}

void CubBackground::draw()
{
    qDebug() << "Background draw" << endl;
    //glActiveTexture(GL_TEXTURE0);
    // set up vertex arrays
    // S'activa la textura i es passa a la GPU
    //texture->bind(0);
    //program->setUniformValue("texMap", 0);
    // Activació a GL del Vertex Buffer Object


    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // Transferència dels punts, colors i coordenades de textura al vertex buffer object
    glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*NumVertices + sizeof(vec2)*NumVertices,
                  NULL, GL_STREAM_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*NumVertices, points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*NumVertices, sizeof(vec2)*NumVertices, vertexsTextura);

    glBindVertexArray( vao );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,  0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,  (void*)(sizeof(point4)*NumVertices));
    glEnableVertexAttribArray(1);

    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_TEXTURE_2D);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glDepthFunc(GL_LESS);


    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDisable(GL_TEXTURE_CUBE_MAP);
}
