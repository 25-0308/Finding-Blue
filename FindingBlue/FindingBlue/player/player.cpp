#include "../Player.h"
#include "../field.h"
#include <GL/glm/gtc/matrix_transform.hpp>
#include<iostream>
void Player::move(float deltaTime)
{
	if (!this->is_alive)return;
    glm::vec3 forward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
	glm::vec3 nextposition = position;
	
    //���� �̵���ΰ� ���̸� �̵����ϰ�
	//�Ϻη� �� ¥ġ�� ������� ��Ȯ�ϰ��Ϸ��� 0.05�� �� �ϸ��
	if (nextposition.x < -2.2f) nextposition.x = -2.2f;
	if (nextposition.z < -2.2f) nextposition.z = -2.2f;
	if (nextposition.x > 97.0f) nextposition.x = 97.0f;
	if (nextposition.z > 97.0f) nextposition.z = 97.0f;
	// �� Ű �Էº��� �浹 üũ
	if (keys['w']) {
		glm::vec3 test_pos = nextposition + forward * speed * deltaTime;
		if (!check_collision(test_pos)) {
			nextposition = test_pos;
		}
	}
	if (keys['s']) {
		glm::vec3 test_pos = nextposition - forward * speed * deltaTime;
		if (!check_collision(test_pos)) {
			nextposition = test_pos;
		}
	}
	if (keys['a']) {
		glm::vec3 test_pos = nextposition - right * speed * deltaTime;
		if (!check_collision(test_pos)) {
			nextposition = test_pos;
		}
	}
	if (keys['d']) {
		glm::vec3 test_pos = nextposition + right * speed * deltaTime;
		if (!check_collision(test_pos)) {
			nextposition = test_pos;
		}
	}

	position = nextposition;

position.y += verticalVelocity * deltaTime;

	if (!isGrounded) {
		verticalVelocity += gravity * deltaTime;
	}


	if (position.y <= 0.0f) {
		position.y = 0.0f;
		verticalVelocity = 0.0f;
		isGrounded = true;
	}
	collision.center = position;
	collision.center.y += 0.4f; 

}
void Player::change_weapon(int index)
{
	if (index >= 0 && index < weapons.size()) {
		currentWeapon = index;
	}
}

void Player::zoom_in(float deltaTime) {
	if (this->zoom_mode) {
		FOV -= 30.0f * deltaTime;
		if (FOV < 35.0f) FOV = 35.0f;

		//�ϴ� �Ѻ��� ��ġ����
		if (!this->weapons.empty())
		this->weapons[currentWeapon]->zoom_in(true, deltaTime);

	}
	else if (!this->zoom_mode) {
		FOV += 30.0f * deltaTime;
		if (FOV > 45.0f) FOV = 45.0f;
		if(!this->weapons.empty())
		this->weapons[currentWeapon]->zoom_in(false, deltaTime);
	}
}
void Player::jump() {
	if (isGrounded) {
		verticalVelocity = jumpPower;  // ���� Ƣ���
		isGrounded = false;
	}
}

bool Player::check_collision(const glm::vec3& next_position) {
	if (!field) return false; // �ʵ� ������ ������ �浹 ����

	// �ݶ��̴� ����
	Collision test_collision = collision;
	test_collision.center = next_position;
	test_collision.center.y += 0.4f;

	// �浹 �˻�
	for (const auto& wall_collision : field->collisions) {
		if (test_collision.check_collision(wall_collision)) {
			std::cout << "�÷��̾�� �� �浹!" << std::endl;
			return true; // �浹 �߻�
		}
	}

	return false; // �浹 �̹߻�
}

void Player::draw_health_bar(GLuint shader) {


	health_bar.draw(shader);

}
void Player::update_health_bar() {
	float health = static_cast<float>(this->health) / 100.0f;
	health_bar.scale.x = (health);
	//체력바가 왼쪽에 있어야하니까 체력비율만큼 왼쪽으로 이동 기본피봇은 10.0
	health_bar.position.z = 10.0 - (1.0-health)*10.0f;
	//std::cout << health_bar.position.z << std::endl;

	//체력이 0이되면 상태 죽음으로
	if (this->health <= 0)
		this->is_alive = false;

}
