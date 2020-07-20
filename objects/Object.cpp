#include "./objects/Object.h"
#include "Material.h"

#include <math.h>
#include <QCoreApplication>

/**
 * @brief Object::Object
 * @param npoints
 * @param parent
 */
Object::Object(int npoints, Material *material, QObject *parent) : QObject(parent){
    numPoints = npoints;
    points = new point4[numPoints];
    normals= new point4[numPoints];
    tangents = new point4[numPoints];
    bitangents = new point4[numPoints];
    textCoordVertex = new vec2[numPoints];
    this->material = material;
    createdWithFile = false;
 }

/**
 * @brief Object::Object
 * @param npoints
 * @param n
 */
Object::Object(int npoints, QString n) : numPoints(npoints){
    points = new point4[numPoints];
    normals= new point4[numPoints];
    tangents = new point4[numPoints];
    bitangents = new point4[numPoints];
    textCoordVertex = new vec2[numPoints];
    parseObjFile(n);
    make();
    createdWithFile = true;
}

Object::Object(QString gizmo, Material *m){
    parseObjFile(gizmo);

    numPoints = 0;
    for(unsigned int i=0; i<cares.size(); i++){
        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++){
            numPoints++;
        }
    }
    points = new point4[numPoints];
    normals= new point4[numPoints];
    tangents = new point4[numPoints];
    bitangents = new point4[numPoints];
    textCoordVertex = new vec2[numPoints];
    make();
    createdWithFile = true;

    material=m;
}

Object::Object(QString gizmo){
    parseObjFile(gizmo);

    numPoints = 0;
    for(unsigned int i=0; i<cares.size(); i++){
        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++){
            numPoints++;
        }
    }
    points = new point4[numPoints];
    normals= new point4[numPoints];
    tangents = new point4[numPoints];
    bitangents = new point4[numPoints];
    textCoordVertex = new vec2[numPoints];
    make();
    createdWithFile = true;
}

Object::Object(){
    points = new point4[10000];
    normals= new point4[10000];
    tangents = new point4[10000];
    bitangents = new point4[10000];
    textCoordVertex = new vec2[10000];
    parseObjFile("://resources/models/sphere0.obj");
    make();
    createdWithFile = true;
}

/**
 * @brief Object::~Object
 */
Object::~Object(){
    delete points;
    delete normals;
    if(createdWithFile) delete material;
}

/**
 * @brief Object::toGPU
 * @param pr
 */
void Object::toGPU(QGLShaderProgram *pr) {
    // TO  DO: A modificar a la fase 1 de la practica 2
    qDebug() << "Obj to GPU.....";

    program = pr;
    // Creació d'un vertex array object

    glGenVertexArrays( 1, &vao );

    // Creacio i inicialitzacio d'un vertex buffer object (VBO)
    glGenBuffers( 1, &buffer );
}


/**
 * Pintat en la GPU.
 * @brief Object::draw
 */
void Object::draw(){
        qDebug() << "Draw normal...";
        //First we set the material we are going to use for this object
        this->material->toGPU(program);
        // Aqui s'ha de fer el pas de dades a la GPU per si hi ha més d'un objecte
        // Activació a GL del Vertex Buffer Object
        glBindBuffer( GL_ARRAY_BUFFER, buffer );

        glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*Index + sizeof(point4)*Index, NULL, GL_STATIC_DRAW ); //Size of vertex + size of normal vertex
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*Index, points ); //Vertex points
        glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index, sizeof(point4)*Index, normals ); //Normals of vertexs

        // set up vertex arrays
        glBindVertexArray( vao );
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,  0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  (void*)(sizeof(point4)*Index));
        glEnableVertexAttribArray(1);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays( GL_TRIANGLES, 0, Index );

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
}

/**
 * @brief Object::make
 */
void Object::make(){

    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal calcular la normal a cada vertex a la CPU
    computeAndSetCentroid();
    calculateTangents();
    Index = 0;

    for(unsigned int i=0; i<cares.size(); i++){
        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++){
            points[Index] = vertexs[cares[i].idxVertices[j]];
            normals[Index] = getNormalAtVertex(cares[i].idxVertices[j]);
            tangents[Index] = vec4(tangentsVertexs[cares[i].idxVertices[j]],0);
            bitangents[Index] = vec4(bitangentsVertexs[cares[i].idxVertices[j]],0);
            if(this->hasTextureVertexs())
                textCoordVertex[Index] = textVertexs[cares[i].idxVertices[j]];
            Index++;
        }
    }
}


void Object::computeAndSetCentroid() {
    float min_x = 999999999999999;
    float min_y = 999999999999999;
    float min_z = 999999999999999;
    float min_w = 999999999999999;
    float max_x = -999999999999999;
    float max_y = -999999999999999;
    float max_z = -999999999999999;
    float max_w = -999999999999999;

    for(point4 vertex : vertexs) {
        if(vertex.x > max_x)
            max_x = vertex.x;
        if(vertex.x < min_x)
            min_x = vertex.x;

        if(vertex.y > max_y)
            max_y = vertex.y;
        if(vertex.y < min_y)
            min_y = vertex.y;

        if(vertex.z > max_z)
            max_z = vertex.z;
        if(vertex.z < min_z)
            min_z = vertex.z;

        if(vertex.w > max_w)
            max_w = vertex.w;
        if(vertex.w < min_w)
            min_w = vertex.w;
    }

    centroid.x = (max_x + min_x)/2.0;
    centroid.y = (max_y + min_y)/2.0;
    centroid.z = (max_z + min_z)/2.0;
    centroid.w = (max_w + min_w)/2.0;
}

/**
 * Pintat en la GPU.
 * @brief Object::drawTexture
 */
void Object::drawTexture(){
    glActiveTexture(GL_TEXTURE0); //Activem la textura 0 : En un futur poden haver-hi més, comprobar
    //First we set the material we are going to use for this object
    this->material->toGPU(program);
    //Secondly we set if we have texture points or not
    program->setUniformValue("hasTextureVertexs", this->hasTextureVertexs());
    //Thirdly we send the centroid that we will use as the center of the containing sphere
    program->setUniformValue("centerOfMass", centroid.x, centroid.y, centroid.z, centroid.w);
    // S'activa la textura i es passa a la GPU
    texture->bind(0);

    program->setUniformValue("texMap", 0);

    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*Index + sizeof(point4)*Index + sizeof(vec2)*Index, NULL, GL_STATIC_DRAW ); //Size of vertex + size of normal vertex + Texture maps

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*Index, points ); //Vertex points
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index, sizeof(point4)*Index, normals ); //Normals of vertexs
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index + sizeof(point4)*Index, sizeof(vec2)*Index, textCoordVertex ); //Texture points


    // set up vertex arrays
    glBindVertexArray( vao );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,  0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  (void*)(sizeof(point4)*Index));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0,  (void*)(2*sizeof(point4)*Index));
    glEnableVertexAttribArray(2);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glDrawArrays( GL_TRIANGLES, 0, Index );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

/**
 * Pintat en la GPU.
 * @brief Object::drawNormalTexture
 */
void Object::drawNormalTexture(){
    glActiveTexture(GL_TEXTURE1); //Activem la textura 0 : En un futur poden haver-hi més, comprobar
    //First we set the material we are going to use for this object
    this->material->toGPU(program);
    //Secondly we set if we have texture points or not
    program->setUniformValue("hasTextureVertexs", this->hasTextureVertexs());
    //Thirdly we send the centroid that we will use as the center of the containing sphere
    program->setUniformValue("centerOfMass", centroid.x, centroid.y, centroid.z, centroid.w);
    // S'activa la textura i es passa a la GPU
    texture->bind(0);
    program->setUniformValue("texMap", 0);
    normaltexture->bind(1);
    program->setUniformValue("texNormalMap", 1);

    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*Index + sizeof(point4)*Index + sizeof(point4)*Index + sizeof(point4)*Index + sizeof(vec2)*Index, NULL, GL_STATIC_DRAW ); //Size of vertex + size of normal vertex + Texture maps

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*Index, points ); //Vertex points
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index, sizeof(point4)*Index, normals ); //Normals of vertexs
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index*2, sizeof(point4)*Index, tangents ); //tangents
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index*3, sizeof(point4)*Index, bitangents ); //tangents
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index*4, sizeof(vec2)*Index, textCoordVertex ); //Texture points


    // set up vertex arrays
    glBindVertexArray( vao );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,  0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  (void*)(sizeof(point4)*Index));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0,  (void*)(2*sizeof(point4)*Index));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0,  (void*)(3*sizeof(point4)*Index));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0,  (void*)(4*sizeof(point4)*Index));
    glEnableVertexAttribArray(4);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glDrawArrays( GL_TRIANGLES, 0, Index );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}


void Object::drawBackgroundReflection(){
    qDebug() << "Draw normal...";
    //First we set the material we are going to use for this object
    this->material->toGPU(program);
    // Aqui s'ha de fer el pas de dades a la GPU per si hi ha més d'un objecte
    // Activació a GL del Vertex Buffer Object
    background_texture->bind(0);
    program->setUniformValue("texEnvironment", 0);

    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*Index + sizeof(point4)*Index, NULL, GL_STATIC_DRAW ); //Size of vertex + size of normal vertex
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*Index, points ); //Vertex points
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index, sizeof(point4)*Index, normals ); //Normals of vertexs

    // set up vertex arrays
    glBindVertexArray( vao );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,  0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  (void*)(sizeof(point4)*Index));
    glEnableVertexAttribArray(1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays( GL_TRIANGLES, 0, Index );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}


void Object::drawBackgroundRefraction(){
    qDebug() << "Draw normal...";
    //First we set the material we are going to use for this object
    this->material->toGPU(program);
    // Aqui s'ha de fer el pas de dades a la GPU per si hi ha més d'un objecte
    // Activació a GL del Vertex Buffer Object
    background_texture->bind(0);
    program->setUniformValue("texEnvironment", 0);

    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*Index + sizeof(point4)*Index, NULL, GL_STATIC_DRAW ); //Size of vertex + size of normal vertex
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*Index, points ); //Vertex points
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index, sizeof(point4)*Index, normals ); //Normals of vertexs

    // set up vertex arrays
    glBindVertexArray( vao );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,  0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  (void*)(sizeof(point4)*Index));
    glEnableVertexAttribArray(1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays( GL_TRIANGLES, 0, Index );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}



void Object::setTexture(QOpenGLTexture *t){
   texture = t;
   initTexture();
}

void Object::setBackgroundTexture(QOpenGLTexture *t){
   background_texture = t;
   initBackgroundTexture();
}

void Object::setNormalTexture(QOpenGLTexture *nt){
    normaltexture = nt;
    initNormalTexture();
}

/**
 * @brief Object::initTexture
 */
void Object::initTexture()
 {
    qDebug() << "Initializing textures... Dffuse";
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    //texture->setWrapMode(QOpenGLTexture::ClampToEdge);
 }

void Object::initBackgroundTexture()
 {
    qDebug() << "Initializing textures...";
    background_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    background_texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
 }

void Object::initNormalTexture(){
    qDebug() << "Initializing normal texture... Normal";
    normaltexture->setMinificationFilter(QOpenGLTexture::Linear);
    normaltexture->setMagnificationFilter(QOpenGLTexture::Linear);
    //normaltexture->setWrapMode(QOpenGLTexture::);
}

bool Object::hasTextureVertexs() {
    return textVertexs.size() != 0;
}

void Object::parseObjFile(const QString &fileName)
{
    QFile file(fileName);
    if(file.exists()) {
        if(file.open(QFile::ReadOnly | QFile::Text)) {
            QVector<QVector3D> v, vn;
            QVector<QVector2D> vt;

            while(!file.atEnd()) {
                QString line = file.readLine().trimmed();
                QStringList lineParts = line.split(QRegularExpression("\\s+"));
                if(lineParts.count() > 0) {
                    // if it’s a comment
                    if(lineParts.at(0).compare("#", Qt::CaseInsensitive) == 0)
                    {
                        qDebug() << line.remove(0, 1).trimmed();
                    }

                    // if it’s a vertex position (v)
                    else if(lineParts.at(0).compare("v", Qt::CaseInsensitive) == 0)
                    {
                        int vertexIndex = vertexs.size();
                        vertexs.push_back(point4(lineParts.at(1).toFloat(),
                                                 lineParts.at(2).toFloat(),
                                                 lineParts.at(3).toFloat(), 1.0f));
                        if(this->facesIndexsOfVertexIndex.count(vertexIndex) == 0) {
                            list<int> emptyList;
                            this->facesIndexsOfVertexIndex.insert(make_pair(vertexIndex, emptyList));
                        }
                    }

                    // if it’s a normal (vn)
                    else if(lineParts.at(0).compare("vn", Qt::CaseInsensitive) == 0)
                    {
                        normalsVertexs.push_back (point4(lineParts.at(1).toFloat(),
                                            lineParts.at(2).toFloat(),
                                            lineParts.at(3).toFloat(), 0.0f));
                    }
                    //if it's a texture file (t)
                    else if(lineParts.at(0).compare("t", Qt::CaseInsensitive) == 0)
                    {
                        lineParts.removeFirst();
                        QString absolutePathToImage = lineParts.join("");
                        QImage imageLoaded = QImage(absolutePathToImage);
                        texture = new QOpenGLTexture(imageLoaded);
                    }
                    else if(lineParts.at(0).compare("tn", Qt::CaseInsensitive) == 0)
                    {
                        lineParts.removeFirst();
                        QString absolutePathToImage = lineParts.join("");
                        QImage imageLoaded = QImage(absolutePathToImage);
                        normaltexture = new QOpenGLTexture(imageLoaded);
                    }
                    // if it’s a texture vertex (vt)
                    else if(lineParts.at(0).compare("vt", Qt::CaseInsensitive) == 0)
                    {
                        textVertexs.push_back(vec2(lineParts.at(1).toFloat(),
                                            lineParts.at(2).toFloat()));
                    }

                    // if it’s face data (f)
                    // there’s an assumption here that faces are all triangles
                    else if(lineParts.at(0).compare("f", Qt::CaseInsensitive) == 0)
                    {
                        Cara *cara = new Cara();
                        // get points from v array
                        int vertex1Index = lineParts.at(1).split("/").at(0).toInt() - 1;
                        cara->idxVertices.push_back(vertex1Index);
                        int vertex2Index = lineParts.at(2).split("/").at(0).toInt() - 1;
                        cara->idxVertices.push_back(vertex2Index);
                        int vertex3Index = lineParts.at(3).split("/").at(0).toInt() - 1;
                        cara->idxVertices.push_back(vertex3Index);

                        //Adding face to list of faces of the vertex
                        int faceIndex = cares.size();
                        this->facesIndexsOfVertexIndex[vertex1Index].push_back(faceIndex);
                        this->facesIndexsOfVertexIndex[vertex2Index].push_back(faceIndex);
                        this->facesIndexsOfVertexIndex[vertex3Index].push_back(faceIndex);

                        if(textVertexs.size() > 0) // check if really there are any UV coords
                        {
                            cara->idxTextures.push_back( lineParts.at(1).split("/").at(1).toInt() - 1);
                            cara->idxTextures.push_back( lineParts.at(2).split("/").at(1).toInt() - 1);
                            cara->idxTextures.push_back( lineParts.at(3).split("/").at(1).toInt() - 1);
                        }

                        // get normals from vn array
                        cara->idxNormals.push_back( lineParts.at(1).split("/").at(2).toInt() - 1);
                        cara->idxNormals.push_back( lineParts.at(2).split("/").at(2).toInt() - 1);
                        cara->idxNormals.push_back( lineParts.at(3).split("/").at(2).toInt() - 1);


                        // cara->calculaNormal(); //TODO: preguntar Te la dan ya y por eso está comentada?
                        cares.push_back(*cara);
                    }
                    //if it's a material (m)
                    else if(lineParts.at(0).compare("m", Qt::CaseInsensitive) == 0)
                    {
                        Material *material = new Material(
                                    Common::vec3(lineParts.at(1).toFloat(), lineParts.at(2).toFloat(), lineParts.at(3).toFloat()), //ambient
                                    Common::vec3(lineParts.at(4).toFloat(), lineParts.at(5).toFloat(), lineParts.at(6).toFloat()), //diffuse
                                    Common::vec3(lineParts.at(7).toFloat(), lineParts.at(8).toFloat(), lineParts.at(9).toFloat()), //specular
                                    lineParts.at(10).toFloat(), //shininess
                                    Common::vec3(lineParts.at(11).toFloat(), lineParts.at(12).toFloat(), lineParts.at(13).toFloat()) //transparency
                                    );
                        this->material = material;
                    }

                }
            }

            file.close();
        }
    }
}

Capsa3D Object::calculCapsa3D()
{
    vec3    pmin, pmax;
    int     i;
    Capsa3D capsa;

    pmin.x = points[0].x;
    pmin.y = points[0].y;
    pmin.z = points[0].z;
    pmax = pmin;
    for(i=1; i<Index; i++) {
        if(points[i].x <pmin[0])
            pmin[0] = points[i].x;
        if(points[i].y <pmin[1])
            pmin[1] = points[i].y;
        if(points[i].z <pmin[2])
            pmin[2] = points[i].z;

        if(points[i].x >pmax[0])
            pmax[0] = points[i].x;
        if(points[i].y >pmax[1])
            pmax[1] = points[i].y;
        if(points[i].z >pmax[2])
            pmax[2] = points[i].z;
    }
    capsa.pmin = pmin;
    capsa.a = pmax[0]-pmin[0];
    capsa.h = pmax[1]-pmin[1];
    capsa.p = pmax[2]-pmin[2];
    return capsa;
}

void Object::aplicaTG(TG *tg){
    Index = 0;
    for(unsigned int i=0; i<vertexs.size(); i++) {
        vertexs[i] = tg->getTG()*vertexs[i];
    }
    for(unsigned int i=0; i<cares.size(); i++){
        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++){
            points[Index] = vertexs[cares[i].idxVertices[j]] ;
            Index++;
        }
    }
    computeAndSetCentroid();
    calculateTangents();
}

point4 Object::getNormalAtVertex(int vertexIndex) {
    list<int> indexsOfvertexFaces = facesIndexsOfVertexIndex[vertexIndex];
    point4 averageNormal = point4(0.0, 0.0, 0.0, 0.0);
    for(int faceIndex: indexsOfvertexFaces)
        averageNormal += this->normalsVertexs[faceIndex];
    averageNormal /= float(indexsOfvertexFaces.size());
    return normalize(averageNormal);
}

void Object::calculateTangents(){
    tangentsVertexs.resize(vertexs.size());
    bitangentsVertexs.resize(vertexs.size());

    for(unsigned int i = 0; i < vertexs.size(); i++){
        tangentsVertexs[i] = 0;
        bitangentsVertexs[i] = 0;
    }

    for(unsigned int i=0; i<cares.size(); i++){
        Cara c = cares[i];
        vec4 v0_ = vertexs[c.idxVertices[0]];
        vec4 v1_ = vertexs[c.idxVertices[1]];
        vec4 v2_ = vertexs[c.idxVertices[2]];
        vec3 v0 = vec3(v0_.x, v0_.y, v0_.z);
        vec3 v1 = vec3(v1_.x, v1_.y, v1_.z);
        vec3 v2 = vec3(v2_.x, v2_.y, v2_.z);

        vec3 deltaPos1 = v1 - v0;
        vec3 deltaPos2 = v2 - v0;
        vec3 center = vec3(centroid.x, centroid.y, centroid.z);
        vec3 d0 = normalize(v0 - center);
        vec3 d1 = normalize(v1 - center);
        vec3 d2 = normalize(v2 - center);
        vec2 uv0, uv1, uv2;
        if(this->hasTextureVertexs()){
            uv0 = textVertexs[c.idxVertices[0]];
            uv1 = textVertexs[c.idxVertices[1]];
            uv2 = textVertexs[c.idxVertices[2]];
        }else{
            float tu0 = 0.5 - atan2(d0.z, d0.x)/(2*M_PI);
            float tv0 = asin(d0.y)/(M_PI) - 0.5;
            float tu1 = 0.5 - atan2(d1.z, d1.x)/(2*M_PI);
            float tv1 = asin(d1.y)/(M_PI) - 0.5;
            float tu2 = 0.5 - atan2(d2.z, d2.x)/(2*M_PI);
            float tv2 = asin(d2.y)/(M_PI) - 0.5;

            uv0 = vec2(tu0, tv0);
            uv1 = vec2(tu1, tv1);
            uv2 = vec2(tu2, tv2);
        }

        vec2 deltaUV1 = uv1 - uv0;
        vec2 deltaUV2 = uv2 - uv0;
        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
        vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;

        for(unsigned int j=0; j<c.idxVertices.size(); j++){
            tangentsVertexs[c.idxVertices[j]] += tangent;
            bitangentsVertexs[c.idxVertices[j]] += bitangent;
        }
    }

    for(unsigned int i = 0; i < vertexs.size(); i++){
        tangentsVertexs[i] = normalize(tangentsVertexs[i]);
        bitangentsVertexs[i] = normalize(bitangentsVertexs[i]);
    }
}
