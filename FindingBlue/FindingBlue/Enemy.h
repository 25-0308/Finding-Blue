#pragma once
#include "Object.h"

class ENEMY {
private:
    //총이 가지고 있어야하는 그런것들임
    glm::vec3 position;
    glm::vec3 front;
	glm::vec3 scale;


public:
    Object head;
    Object body;
    Object Rarm1;
	Object Rarm2;
	Object Larm1;
	Object Larm2;
	Object Rleg1;
	Object Rleg2;
	Object Lleg1;
	Object Lleg2;

    ENEMY()
		: head("asset/enemy/blue/head.obj", "asset/enemy/blue/blue_enemy.png"),
		body("asset/enemy/blue/body.obj", "asset/enemy/blue/blue_enemy.png"),
		Rarm1("asset/enemy/blue/Rarm1.obj", "asset/enemy/blue/blue_enemy.png"),
		Rarm2("asset/enemy/blue/Rarm2.obj", "asset/enemy/blue/blue_enemy.png"),
		Larm1("asset/enemy/blue/Larm1.obj", "asset/enemy/blue/blue_enemy.png"),
		Larm2("asset/enemy/blue/Larm2.obj", "asset/enemy/blue/blue_enemy.png"),
		Rleg1("asset/enemy/blue/Rleg1.obj", "asset/enemy/blue/blue_enemy.png"),
		Rleg2("asset/enemy/blue/Rleg2.obj", "asset/enemy/blue/blue_enemy.png"),
		Lleg1("asset/enemy/blue/Lleg1.obj", "asset/enemy/blue/blue_enemy.png"),
		Lleg2("asset/enemy/blue/Lleg2.obj", "asset/enemy/blue/blue_enemy.png")
    {
    }

    void init() {
		head.init();
		body.init();
		Rarm1.init();
		Rarm2.init();
		Larm1.init();
		Larm2.init();
		Rleg1.init();
		Rleg2.init();
		Lleg1.init();
		Lleg2.init();
		this->position = glm::vec3(10.0f, -0.625f, 10.0f);
		this->scale = glm::vec3(0.3f);
		head.scale = body.scale = Rarm1.scale = Rarm2.scale = Larm1.scale = Larm2.scale = Rleg1.scale = Rleg2.scale = Lleg1.scale = Lleg2.scale = this->scale;

		//나머지 파츠들도	위치 맞춰주기

    }

    void draw(GLuint shader) {
		head.draw(shader);
		body.draw(shader);
		Rarm1.draw(shader);
		Rarm2.draw(shader);
		Larm1.draw(shader);
		Larm2.draw(shader);
		Rleg1.draw(shader);
		Rleg2.draw(shader);
		Lleg1.draw(shader);
		Lleg2.draw(shader);

    }
    //여기 아래로는 cpp 파일에 작성할 것들
    void update(float deltaTime,glm::vec3 target);




};

