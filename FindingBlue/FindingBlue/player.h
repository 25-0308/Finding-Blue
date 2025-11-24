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

    bool keys[256] = { false };

    float speed = 5.0f;

    Player() :
        position(3.0f, 0.0f, 5.0f),
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
    void zoom_in(float deltaTime);
};
