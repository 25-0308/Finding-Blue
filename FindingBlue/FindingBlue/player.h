#pragma once
#pragma once
#include <GL/glm/glm.hpp>
#include<vector>
#include"weapon.h"
class Player {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
	GLfloat FOV = 45.0f;
    bool zoom_mode = false;

    std::vector<Weapon*> weapons;
    int currentWeapon = 0;
    // --- 점프 관련 ---
    bool isGrounded = true;      // 바닥 위인지
    float verticalVelocity = 0.0f; // y축 속도
    float gravity = -9.8f;        // 중력
    float jumpPower = 6.0f;       // 점프 세기

    bool keys[256] = { false };
	bool mouses[4] = { false }; //좌클 우클 휠위 휠아래
    float speed = 5.0f;

    Player() :
        position(0.8f, 0.0f, 5.0f),
        front(0.0f, 0.0f, -1.0f),
        up(0.0f, 1.0f, 0.0f)
    {
    }

    void move(float deltaTime);
    void change_weapon(int index);
	void draw_weapon(GLuint shader) {
		if (!weapons.empty()) {
			weapons[currentWeapon]->draw(shader);
		}
	}
	glm::vec3 get_player_pos() {
		return position;
	}
    void zoom_in(float deltaTime);
    void jump();
};
