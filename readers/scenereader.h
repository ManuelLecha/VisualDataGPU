#ifndef SCENEREADER_H
#define SCENEREADER_H




#include <QObject>
#include <QString>
#include <QStringList>
#include "scenes/Scene.h"
#include "Material.h"
#include "objects/Object.h"
#include "colormaps/ColorMapStatic.h"

class scenereader
{
public:
    scenereader(Scene * s);


    void readFile(QString fileName);

    void fileLineRead(QString lineReaded);

    Scene *scene;

    void dataFound(QStringList list);

    void frameNumberFound(QStringList list);

    void frameFound(QStringList list);

    void materialFound(QStringList lineParts);

private:
    QString monkey = "://resources/models/Monkey.obj";
    QString sphere = "://resources/models/sphere0.obj";
    int currentFrame=0;
    int maxFrames=10;
    vec3 lastPosition=vec3(0,0,0);
    float lastScale = 0;
    Object* lastObject = nullptr;
};

#endif // SCENEREADER_H
