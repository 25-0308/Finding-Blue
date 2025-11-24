#include"bullet.h"


void BULLET::update(float deltaTime, float yaw, float pitch) {
	//총알 이동
	float speed = 100.0f; // 총알 속도
	this->position += this->front * speed * deltaTime;
	bullet_obj.position = this->position;
	//총알 위치 업데이트
	this->bullet_obj.position = this->position;
	//총알 방향 설정
	if (this->initialized == false) {
		this->bullet_obj.rotation.y = -glm::radians(yaw);
		this->bullet_obj.rotation.z = glm::radians(pitch);
		this->initialized = true;
	}

}