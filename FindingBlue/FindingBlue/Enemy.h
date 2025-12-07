#pragma once
#include "Object.h"
#include"Collision.h"
#include<iostream>
#include<vector>
#include "field.h"

class FIELD;

class BLOOD {
private:
	glm::vec3 position;
	glm::vec3 velocity;  // 파티클의 움직임 (dx,dy,dz)
	float lifetime = 1.0f;   // 1초 뒤 사라지게 예시
	bool active = true;

public:
	Object blood;
	
	BLOOD()
		: blood("asset/blood/blood.obj", "asset/blood/blood.png") {
	}

	void init(glm::vec3 startPos, glm::vec3 vel) {
		blood.init();
		position = startPos;
		blood.scale = glm::vec3(0.2f);
		velocity = vel;
		lifetime = 1.0f;
		active = true;
	}

	void update(float dt) {
		if (!active) return;

		position += velocity * dt;
		blood.position = position;

		lifetime -= dt;
		if (lifetime <= 0.0f) {
			active = false;
		}
	}

	void draw(GLuint shader) {
		if (active)
			blood.draw(shader);
	}

	bool is_dead() const { return !active; }
};
static const char* getTexturePath(int t);
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


	//체력과 사망관련
	int health = 100;
	bool is_dead = false;
	
	int type; //1은 파랑 2는 흰색

	bool dead_sound = false;
public:
	FIELD* field_ref = nullptr;
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
	std::vector<BLOOD*> bloods;
	Collision collision;
	
    ENEMY(int type)
		:type(type), head("asset/enemy/blue/head.obj", getTexturePath(type)),
		body("asset/enemy/blue/body.obj", getTexturePath(type)),
		Rarm1("asset/enemy/blue/Rarm1.obj", getTexturePath(type)),
		Rarm2("asset/enemy/blue/Rarm2.obj", getTexturePath(type)),
		Larm1("asset/enemy/blue/Larm1.obj", getTexturePath(type)),
		Larm2("asset/enemy/blue/Larm2.obj", getTexturePath(type)),
		Rleg1("asset/enemy/blue/Rleg1.obj", getTexturePath(type)),
		Rleg2("asset/enemy/blue/Rleg2.obj", getTexturePath(type)),
		Lleg1("asset/enemy/blue/Lleg1.obj", getTexturePath(type)),
		Lleg2("asset/enemy/blue/Lleg2.obj", getTexturePath(type))
    {
    }

    void init(glm::vec3 position,int type) {
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
		this->type = type;
	
		this->position = glm::vec3(position.x, position.y, position.z);
		this->scale = glm::vec3(0.3f);
		head.scale = body.scale = Rarm1.scale = Rarm2.scale = Larm1.scale = Larm2.scale = Rleg1.scale = Rleg2.scale = Lleg1.scale = Lleg2.scale = this->scale;
		head.position = body.position = Rarm1.position = Rarm2.position = Larm1.position = Larm2.position = Rleg1.position = Rleg2.position = Lleg1.position = Lleg2.position = this->position;
		
		//콜라이더 크기 설정
		collision.center = this->position;
		collision.center.y += 0.5f;
		collision.halfsize = collision.halfsize * 0.25f;
		collision.halfsize.y += 0.3f;

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
		body.draw(shader);
		Rarm1.draw(shader);
		Rarm2.draw(shader);
		Larm1.draw(shader);
		Larm2.draw(shader);
		Rleg1.draw(shader);
		Rleg2.draw(shader);
		Lleg1.draw(shader);
		Lleg2.draw(shader);
		if (this->is_dead) {
			for (auto& b : bloods)
				b->draw(shader);

		}
		collision.Debug_Draw();
    }
	void set_field_reference(FIELD* field) { field_ref = field; }
    //여기 아래로는 cpp 파일에 작성할 것들
	bool check_collision(const glm::vec3& next_position);
	bool update(float deltaTime,glm::vec3 target);
    void run(float deltaTime,glm::vec3 target);
	void idle(float deltaTime);
	bool hit(glm::vec3 pos) {
		float distance = glm::length(pos - this->position);
		return distance < 2.0f;
	}
	void take_damage(int damage);
	void spawn_blood();
	//void death_motion(float deltaTime);
	int get_type() {
		return this->type;
	}


};

glm::vec3 rotateAroundPivot(glm::vec3 point, glm::vec3 pivot, float angleZ);





static const char* getTexturePath(int t) {
	switch (t) {
	case 1: return "asset/enemy/blue/blue_enemy.png";
	case 2: return "asset/enemy/blue/white_enemy.png";

	}
	return "";
}