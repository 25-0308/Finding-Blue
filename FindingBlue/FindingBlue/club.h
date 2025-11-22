#pragma once

#include "Object.h"

class CLUB {
private:
    //빠따 가지고 있어야하는 그런것들임
    glm::vec3 position;
    glm::vec3 front;



public:
    Object hand;
    Object metal;

    CLUB()
        : hand("asset/bbadda/bbadda.obj", "asset/ak_47/ak_v2.png"),
        metal("asset/bbadda/hand.obj", "asset/ak_47/ak_v2.png")
    {
    }

    void init() {
        hand.init();
        metal.init();
        hand.scale = metal.scale = glm::vec3(0.1f);

    }

    void draw(GLuint shader) {

        hand.draw(shader);
        metal.draw(shader);
    }
    //여기 아래로는 cpp 파일에 작성할 것들
    void update(float deltaTime, glm::vec3 position, float yaw, float pitch);




};

