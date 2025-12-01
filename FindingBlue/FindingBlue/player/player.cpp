#include "../Player.h"
#include "../field.h"
#include <GL/glm/gtc/matrix_transform.hpp>
#include<iostream>
void Player::move(float deltaTime)
{
    glm::vec3 forward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
	glm::vec3 nextposition = position;
	
    //다음 이동경로가 벽이면 이동못하게
	//일부러 좀 짜치게 만들었음 정확하게하려면 0.05씩 다 하면됨
	if (nextposition.x < -2.2f) nextposition.x = -2.2f;
	if (nextposition.z < -2.2f) nextposition.z = -2.2f;
	if (nextposition.x > 97.0f) nextposition.x = 97.0f;
	if (nextposition.z > 97.0f) nextposition.z = 97.0f;
	// 각 키 입력별로 충돌 체크
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

		//일단 총부터 위치변경
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

bool Player::check_collision(const glm::vec3& next_position) {
	if (!field) return false; // 필드 참조가 없으면 충돌 없음

	// 콜라이더 생성
	Collision test_collision = collision;
	test_collision.center = next_position;
	test_collision.center.y += 0.4f;

	// 충돌 검사
	for (const auto& wall_collision : field->collisions) {
		if (test_collision.check_collision(wall_collision)) {
			std::cout << "플레이어와 벽 충돌!" << std::endl;
			return true; // 충돌 발생
		}
	}

	return false; // 충돌 미발생
}