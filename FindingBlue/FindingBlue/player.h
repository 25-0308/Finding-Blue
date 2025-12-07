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
    // --- ���� ���� ---
    bool isGrounded = true;      // �ٴ� ������
    float verticalVelocity = 0.0f; // y�� �ӵ�
    float gravity = -9.8f;        // �߷�
    float jumpPower = 6.0f;       // ���� ����

    bool keys[256] = { false };
	bool mouses[4] = { false }; //��Ŭ ��Ŭ ���� �پƷ�
    float speed = 5.0f;

    Collision collision;
    FIELD* field = nullptr; // ���� �浹���� ����

    //체력
	int health = 100;
	Object health_bar;
	bool is_alive = true;
   
	float hit_timer = 0.0f;

    Player() :
        position(0.8f, 0.0f, 5.0f),//<-������ �̰�
		//position(10.0f, 0.0f, 5.0f),    //������
        front(0.0f, 0.0f, -1.0f),
        up(0.0f, 1.0f, 0.0f),
        collision(glm::vec3(0.3f,0.8f,0.3f)),
        health_bar("asset/UI/health_bar.obj", "asset/UI/health_bar.png")
    {
    }
    void init() {
		health_bar.init();
		health_bar.position = glm::vec3(1.0,0.0f, 5.0f);
		health_bar.rotation.y = glm::radians(270.0f);
        health_bar.scale = glm::vec3(1.0f);
    
    }
    void update(float deltaTime) {
		hit_timer -= deltaTime;
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
    //�浹 �Լ�
    void set_field(FIELD* field_ref) { field = field_ref; }
    bool check_collision(const glm::vec3& next_position);

    void draw_health_bar(GLuint shader);
    void update_health_bar();
	int get_ammo() {
		if (weapons.empty()) return 0;
		return weapons[currentWeapon]->ammo;
	}
    void get_damage(int damage) {
        health -= damage;
		hit_timer = 1.0f;
        if (health <= 0) {
            health = 0;
        }
    }
};
