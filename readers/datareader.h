#ifndef DATAREADER_H
#define DATAREADER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "scenes/Scene.h"
#include "Material.h"
#include "objects/Object.h"
#include "colormaps/ColorMapStatic.h"

struct Range {
    double min;
    double max;

    Range(double min, double max) {
        this->min = min;
        this->max = max;
    }

    Range() {
        this->min = 0;
        this->max = 0;
    }
};
class DataReader
{
public:
    DataReader(Scene *s);

    DataReader(Scene *s, double x_max, double x_min, double z_max, double z_min, double s_min, double s_max, QString groundTexture);

    void readFile(QString fileName);

    void fileLineRead(QString lineReaded);

    Scene *scene;

    void baseFound(QStringList list);

    void limitsFound(QStringList list);

    void propFound(QStringList list);

    void dataFound(QStringList list);

    Object *createDataObject(double x, double z, double s);

    QString gizmo = "://resources/models/sphere0.obj";

    ColorMapStatic cmap = ColorMapStatic(ColorMapStatic::COLOR_MAP_TYPE_INFERNO);

    QString groundTexture = "://resources/textures/mosaic.png";

private:
    Range data_xlim;
    Range data_zlim;
    Range data_slim;

    Range scene_xlim;
    Range scene_zlim;
    Range scene_slim;
    int animations;
    int framesPerAnimation;
    Material defaultMaterial;
};

#endif // DATAREADER_H
