#include "../Player.h"
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
	if (keys['w']) {
		nextposition += forward * speed * deltaTime;
	}
	if (keys['s']) {
		nextposition -= forward * speed * deltaTime;
	}
	if (keys['a']) {
		nextposition -= right * speed * deltaTime;
	}
	if (keys['d']) {
		nextposition += right * speed * deltaTime;
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
void Player::jump() {
	if (isGrounded) {
		verticalVelocity = jumpPower;  // 위로 튀기기
		isGrounded = false;
	}
}
