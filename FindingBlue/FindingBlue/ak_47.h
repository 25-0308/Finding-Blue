#pragma once
#include "Object.h"

class AK_47 {
private:
    //총이 가지고 있어야하는 그런것들임
	glm::vec3 position;
	glm::vec3 front;    



public:
    Object wood;
    Object metal;

    AK_47()
        : wood("asset/ak_47/ak_47_wood.obj", "asset/ak_47/ak_v2.png"),
        metal("asset/ak_47/ak_47_metal.obj", "asset/ak_47/ak_v2.png")
    {
    }

    void init() {
        wood.init();
        metal.init();
		wood.scale = metal.scale = glm::vec3(0.3f);

    }

    void draw(GLuint shader) {
        
        wood.draw(shader);
        metal.draw(shader);
    }
	//여기 아래로는 cpp 파일에 작성할 것들
    void update(float deltaTime, glm::vec3 position, float yaw, float pitch);




};

