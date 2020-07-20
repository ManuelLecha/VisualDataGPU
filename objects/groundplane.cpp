#include "groundplane.h"

GroundPlane::GroundPlane(float width, float height, QString textureFile)
{
    vertices[0] = point4(-width, 0, height,1.0);
    vertices[1] = point4(width, 0, height,1.0);
    vertices[2] = point4(-width, 0, -height,1.0);
    vertices[3] = point4(width, 0, -height,1.0);

    vertex_colors[0] =    color4( 0.2, 0.2, 0.7, 1.0 );
    vertex_colors[1] =    color4( 0.2, 0.2, 0.7, 1.0 );
    vertex_colors[2] =    color4( 0.8, 0.8, 0.8, 1.0 );
    vertex_colors[3] =    color4( 0.8, 0.8, 0.8, 1.0 );
    this->textureFile = textureFile;
}

GroundPlane::GroundPlane(){
    float width = 1.0;
    float height = 1.0;

    vertices[0] = point4(-width, 0, height,1.0);
    vertices[1] = point4(width, 0, height,1.0);
    vertices[2] = point4(-width, 0, -height,1.0);
    vertices[3] = point4(width, 0, -height,1.0);


    vertex_colors[0] =    color4( 0.2, 0.2, 0.7, 1.0 );
    vertex_colors[1] =    color4( 0.2, 0.2, 0.7, 1.0 );
    vertex_colors[2] =    color4( 0.8, 0.8, 0.8, 1.0 );
    vertex_colors[3] =    color4( 0.8, 0.8, 0.8, 1.0 );
}

void GroundPlane::quad( int a, int b, int c, int d )
{
    points[Index] = vertices[a];
    vertexsTextura[Index] = vec2(0.0, 0.0); Index++;
    points[Index] = vertices[b];
    vertexsTextura[Index] = vec2(1.0, 0.0); Index++;
    points[Index] = vertices[c];
    vertexsTextura[Index] = vec2(1.0, 1.0); Index++;
    points[Index] = vertices[a];
    vertexsTextura[Index] = vec2(0.0, 0.0); Index++;
    points[Index] = vertices[c];
    vertexsTextura[Index] = vec2(1.0, 1.0); Index++;
    points[Index] = vertices[d];
    vertexsTextura[Index] = vec2(0.0, 1.0); Index++;
}

void GroundPlane::make()
{
    // generacio de la geometria dels triangles per a visualitzar-lo
    Index = 0;
    quad( 1, 0, 2, 3);
    initTextura();
}

void GroundPlane::initTextura()
{
     // Carregar la textura
     glActiveTexture(GL_TEXTURE0);
     // texture = new QOpenGLTexture(QImage(textureFile));
     texture = new QOpenGLTexture(QImage(textureFile));
     texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
     texture->setMagnificationFilter(QOpenGLTexture::Linear);
     texture->bind(0);
}

void GroundPlane::toGPU(QGLShaderProgram *program){
    qDebug() << "Passo el ground a la GPU.....\n";

    this->program = program;
    glGenVertexArrays( 1, &vao );
    // Creacio i inicialitzacio d'un vertex buffer object (VBO)
    glGenBuffers( 1, &buffer );
}

void GroundPlane::draw()
{
    texture->bind(0);
    program->setUniformValue("texMap", 0);

    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // Transferència dels punts, colors i coordenades de textura al vertex buffer object
    glBufferData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(vertexsTextura),
                  NULL, GL_STREAM_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(vertexsTextura), vertexsTextura);

    glBindVertexArray( vao );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,  0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,  (void*)(sizeof(point4)*6));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays( GL_TRIANGLES, 0, 6 );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

}
