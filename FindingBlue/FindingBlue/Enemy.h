#pragma once
#include "Object.h"
#include"Collision.h"
#include<iostream>
class ENEMY {
private:
    //총이 가지고 있어야하는 그런것들임
    glm::vec3 position;
    glm::vec3 front;
	glm::vec3 scale;
	bool Rarm1_rotate_dir = true;
	bool Larm1_rotate_dir = false;
	float Rarm1_rotate_angle = 0.0f;
	float Larm1_rotate_angle = 0.0f;
	bool Rarm2_rotate_dir = true;
	float Rarm2_rotate_angle = 0.0f;
	bool Larm2_rotate_dir = false;
	float Larm2_rotate_angle = 0.0f;

	bool body_bobbing_dir = true;
	float body_bobbing_angle = 0.0f;
	Collision collision;

	//다리부분
	bool Rleg1_rotate_dir = true;
	float Rleg1_rotate_angle = 0.0f;
	bool Lleg1_rotate_dir = false;
	float Lleg1_rotate_angle = 0.0f;
	bool Rleg2_rotate_dir = true;
	float Rleg2_rotate_angle = 0.0f;
	bool Lleg2_rotate_dir = false;
	float Lleg2_rotate_angle = 0.0f;
	bool initialized = false;

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

    void init(glm::vec3 position) {
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
	
		this->position = glm::vec3(position.x, position.y, position.z);
		this->scale = glm::vec3(0.3f);
		head.scale = body.scale = Rarm1.scale = Rarm2.scale = Larm1.scale = Larm2.scale = Rleg1.scale = Rleg2.scale = Lleg1.scale = Lleg2.scale = this->scale;
		head.position = body.position = Rarm1.position = Rarm2.position = Larm1.position = Larm2.position = Rleg1.position = Rleg2.position = Lleg1.position = Lleg2.position = this->position;
		collision.center = this->position;

		//나머지 파츠들도	위치 맞춰주기(피봇조절)
		Rarm1.pivot = glm::vec3(0.0f, 0.6f, 0.0f);
		Larm1.pivot = glm::vec3(0.0f, 0.6f, 0.0f);
		//Rarm1.rotation.z = glm::radians(180.0f);
		//Larm1.rotation.z = glm::radians(180.0f);
		Rarm2.pivot = glm::vec3(0.0f, 0.55f, 0.0f);
		Larm2.pivot = glm::vec3(0.0f, 0.55f, 0.0f);
		//Rarm2.rotation.z = glm::radians(180.0f);
		//Larm2.rotation.z = glm::radians(180.0f);

		//다리
		Rleg1.pivot = glm::vec3(0.0f, 0.2f, 0.0f);
		Lleg1.pivot = glm::vec3(0.0f, 0.2f, 0.0f);
		//Rleg1.rotation.z = glm::radians(180.0f);
		//Lleg1.rotation.z = glm::radians(180.0f);
		Rleg2.pivot = glm::vec3(0.0f, 0.1f, 0.0f);
		Lleg2.pivot = glm::vec3(0.0f, 0.1f, 0.0f);

    }

    void draw(GLuint shader) {
		head.draw(shader);
		//body.draw(shader);
		Rarm1.draw(shader);
		Rarm2.draw(shader);
		Larm1.draw(shader);
		Larm2.draw(shader);
		Rleg1.draw(shader);
		Rleg2.draw(shader);
		Lleg1.draw(shader);
		Lleg2.draw(shader);
		collision.Debug_Draw();
    }
    //여기 아래로는 cpp 파일에 작성할 것들
	void update(float deltaTime,glm::vec3 target);
    void run(float deltaTime,glm::vec3 target);
	void idle(float deltaTime);
	bool hit(glm::vec3 pos) {
		float distance = glm::length(pos - this->position);
		return distance < 2.0f;
	}



};

glm::vec3 rotateAroundPivot(glm::vec3 point, glm::vec3 pivot, float angleZ);