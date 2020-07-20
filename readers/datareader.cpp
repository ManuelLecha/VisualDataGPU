#include "datareader.h"
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <sstream>
#include "objects/plane.h"
#include "objects/Object.h"
#include "./TG/scale.h"

DataReader::DataReader(Scene *s, double x_max, double x_min, double z_max, double z_min, double s_min, double s_max, QString groundTexture)
{
    scene = s;
    scene_xlim = Range(x_min, x_max);
    scene_zlim = Range(z_min, z_max);
    scene_slim = Range(s_min, s_max);
    float width = (x_max - x_min)/2;
    float height = (z_max - z_min)/2;

    defaultMaterial = Material();
    GroundPlane *ground = new GroundPlane(width, height, groundTexture);
    ground->make();
    //scene->ground = ground;
}

void DataReader::readFile(QString fileName) {
    QFile file(fileName);
    std::cout<<"data readFile"<< endl;
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Error opening the file" << std::endl;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        fileLineRead(line);
    }

    file.close();
}

void DataReader::fileLineRead(QString lineReaded) {

    QStringList fields = lineReaded.split(",");
    for (auto &str : fields) str = str.trimmed();

    std::cout << fields[0].toStdString() << std::endl;
    if (QString::compare("limits", fields[0], Qt::CaseInsensitive) == 0)
        limitsFound(fields);
    else if (QString::compare("prop", fields[0], Qt::CaseInsensitive) == 0)
        propFound(fields);
    else if (QString::compare("data", fields[0], Qt::CaseInsensitive) == 0)
        dataFound(fields);
    else
        std::cerr << "Element unknown" << fields[0].toStdString() << std::endl;
}

void DataReader::limitsFound(QStringList fields) {
    // limits xmin xmax zmin zmax
    if (fields.size() != 5) {
        std::cerr << "Wrong limits format" << std::endl;
        return;
    }
    data_xlim = Range(fields[1].toDouble(), fields[2].toDouble());
    data_zlim = Range(fields[3].toDouble(), fields[4].toDouble());
}

void DataReader::propFound(QStringList fields) {
    //prop vmin vmax nvalue
    if (fields.size() != 4) {
        std::cerr << "Wrong propietat format" << std::endl;
        return;
    }
    data_slim= Range(fields[1].toDouble(), fields[2].toDouble());
    animations = fields[3].toInt();
}

void DataReader::dataFound(QStringList fields) {
    //data x z valor1 valor2...
    if (fields.size() < 4) {
        std::cerr << "Wrong data format" << std::endl;
        return;
    }

    //float dataS = fields[3].toDouble();
    float *dataS = new float[animations];
    for(int i = 0; i < animations; i++){
        float ds = fields[3+i].toDouble();
        float rs = (ds - data_slim.min) / (data_slim.max - data_slim.min);
        dataS[i] = scene_slim.min + rs * (scene_slim.max - scene_slim.min);
    }

    float dx = fields[1].toDouble(), dz = fields[2].toDouble();
    double rx = (dx - data_xlim.min) / (data_xlim.max - data_xlim.min);
    double rz = (dz - data_zlim.min) / (data_zlim.max - data_zlim.min);
    double sx = scene_xlim.min + rx * (scene_xlim.max - scene_xlim.min);
    double sz = scene_zlim.min + rz * (scene_zlim.max - scene_zlim.min);

    Object *o;
    o = createDataObject(sx, sz, dataS[0]);
    float prevS = dataS[0];
    float nextS;
    int currentFrame = 0;
    for(int i = 1; i < animations; i++){
        nextS = dataS[i];
        float s = nextS/prevS;
        prevS = nextS;
        Animation *a = new Animation();
        a->transf = new ScaleTG(vec3(s,s,s), vec3(sx, 0, sz));
        a->frameIni = currentFrame;
        currentFrame += 1;
        a->frameFinal = currentFrame;
        o->addAnimation(a);
    }

    scene->addObject(o);
}

Object *DataReader::createDataObject(double x, double z, double s) {

    Object *o = new Object(10000, gizmo);
    vec3 color = cmap.getColor(s*255);
    o->material->diffuse = color;
    o->material->ambient = color*0.1;
    qDebug()<<"Material "<<color.x<<" "<<color.y<<" "<<color.z<<" "<<s;

    mat4 translacio = mat4(Translate(vec3(x,0,z)))*Scale(vec3(s,s,s));
    TG transformacio = TG();
    transformacio.matTG = translacio;
    o->aplicaTG(&transformacio);
    return o;
}


