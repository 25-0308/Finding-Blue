#pragma once
#pragma once
#include <GL/glm/glm.hpp>
#include<vector>
#include"weapon.h"
#include "Collision.h"

class FIELD;

class Player {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
	GLfloat FOV = 45.0f;
    bool zoom_mode = false;

    std::vector<Weapon*> weapons;
    int currentWeapon = 0;

    bool keys[256] = { false };
	bool mouses[4] = { false }; //좌클 우클 휠위 휠아래
    float speed = 5.0f;

    Collision collision;
    FIELD* field = nullptr; // 벽과 충돌위해 참조

    Player() :
        position(0.8f, 0.0f, 5.0f),
        front(0.0f, 0.0f, -1.0f),
        up(0.0f, 1.0f, 0.0f),
        collision(glm::vec3(0.3f,0.8f,0.3f))
    {
    }

    void move(float deltaTime);
    void change_weapon(int index);
	void draw_weapon(GLuint shader) {
		if (!weapons.empty()) {
			weapons[currentWeapon]->draw(shader);
		}
	}
    void zoom_in(float deltaTime);

    //충돌 함수
    void set_field(FIELD* field_ref) { field = field_ref; }
    bool check_collision(const glm::vec3& next_position);
};
