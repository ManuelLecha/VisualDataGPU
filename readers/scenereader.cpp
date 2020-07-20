#include "scenereader.h"


#include "datareader.h"
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <sstream>
#include "objects/plane.h"
#include "objects/Object.h"
#include <TG/TG.h>
#include <TG/scale.h>
#include <TG/Translation.h>

scenereader::scenereader(Scene * s)
{
    scene = s;
}

void scenereader::readFile(QString fileName) {
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

void scenereader::fileLineRead(QString lineReaded) {

    QStringList fields = lineReaded.split(",");
    for (auto &str : fields) str = str.trimmed();

    std::cout << fields[0].toStdString() << std::endl;
    if (QString::compare("frames", fields[0], Qt::CaseInsensitive) == 0)
        frameNumberFound(fields);
    else if (QString::compare("f", fields[0], Qt::CaseInsensitive) == 0)
        frameFound(fields);
    else if (QString::compare("data", fields[0], Qt::CaseInsensitive) == 0)
        dataFound(fields);
    else if (QString::compare("m", fields[0], Qt::CaseInsensitive) == 0)
        materialFound(fields);
    else
        std::cerr << "Element unknown" << fields[0].toStdString() << std::endl;
}

void scenereader::dataFound(QStringList fields) {
    //data gizmo
    if (fields.size() < 2) {
        std::cerr << "Wrong data format" << std::endl;
        return;
    }

    if (QString::compare("monkey", fields[1], Qt::CaseInsensitive) == 0){
        lastObject = new Object(10000, monkey);
        scene->addObject(lastObject);
    }else if (QString::compare("sphere", fields[1], Qt::CaseInsensitive) == 0){
        lastObject = new Object(10000, sphere);
        scene->addObject(lastObject);
    }

    currentFrame = 0;
    lastPosition = vec3(0,0,0);
    lastScale = 0;
}

void scenereader::frameFound(QStringList list){
    if (list.size() < 5) {
        std::cerr << "Wrong data format" << std::endl;
        return;
    }
    vec3 pos = vec3(list[1].toDouble(), list[2].toDouble(), list[3].toDouble());
    float scale = list[4].toDouble();

    if(currentFrame == 0){
        TG* trans = new TG();
        trans->matTG = Translate(pos)*Scale(vec3(scale,scale,scale));
        lastObject->aplicaTG(trans);
        lastPosition = pos;
        lastScale = scale;
        currentFrame+=1;
    }else{
        TG* trans = new TG();
        float scaleR = scale/lastScale;
        trans->matTG = Translate(pos)*Scale(vec3(scaleR, scaleR, scaleR))*Translate(-lastPosition) ;
        Animation *a = new Animation();
        a->frameIni = currentFrame;
        a->frameFinal = ++currentFrame;
        a->transf = trans;
        lastObject->addAnimation(a);
        lastPosition = pos;
        lastScale = scale;
    }
}

void scenereader::frameNumberFound(QStringList list){
    if (list.size() < 2) {
        std::cerr << "Wrong data format" << std::endl;
        return;
    }
    maxFrames = list[1].toInt();
}

void scenereader::materialFound(QStringList lineParts){
    if (lineParts.size() < 14) {
        std::cerr << "Wrong data format" << std::endl;
        return;
    }
    Material *material = new Material(
                Common::vec3(lineParts.at(1).toFloat(), lineParts.at(2).toFloat(), lineParts.at(3).toFloat()), //ambient
                Common::vec3(lineParts.at(4).toFloat(), lineParts.at(5).toFloat(), lineParts.at(6).toFloat()), //diffuse
                Common::vec3(lineParts.at(7).toFloat(), lineParts.at(8).toFloat(), lineParts.at(9).toFloat()), //specular
                lineParts.at(10).toFloat(), //shininess
                Common::vec3(lineParts.at(11).toFloat(), lineParts.at(12).toFloat(), lineParts.at(13).toFloat()) //transparency
                );
    scene->elements.back()->material=material;
}




