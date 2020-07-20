#ifndef BUILDER_H
#define BUILDER_H

#include <QtWidgets>
#include <GLWidget.h>

class Builder : public QObject {
        Q_OBJECT

    Scene *scene;
    GLWidget *glWidget;

public:
    Builder(GLWidget *glWid);

public slots:
    void newObjFromFile();
    void newVirtualScene();
    void newDataScene(QString data, QString gizmo, QString texture, QString colormap, double xmin, double xmax, double zmin, double zmax);
    void newDataSceneUI(QString filename, QString gizmo, QString texture, QString colormap);
    void newVirtualSceneUI();
    void newSceneDemo1();
    void newSceneDemo2();

signals:
    void newObj(Object *o);
    void newScene(Scene *sc);
    void newData(Scene *sc);
    void demo1(Scene *sc);
    void demo2(Scene *sc);
};

#endif // BUILDER_H
