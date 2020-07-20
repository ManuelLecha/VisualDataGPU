#include "DataAnimation.h"

DataAnimation::DataAnimation(){
}

void DataAnimation::getScene(){
    Scene *scene = new Scene();
    oneSphere(scene);
}

void DataAnimation::oneSphere(Scene *s){
    QString s0("://resources/models/sphere0.obj");
    Object * obj0 = new Object(100000, s0);
    Animation *anim = new Animation();
    anim->transf = new Translation(vec3(0.2, 0.0, 0.0));
    obj0->addAnimation(anim);
    s->addObject(obj0);
}




