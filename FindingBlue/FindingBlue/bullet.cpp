#include"bullet.h"


void BULLET::update(float deltaTime) {
	//총알 이동
	float speed = 50.0f; // 총알 속도
	this->position += this->front * speed * deltaTime;
	bullet_obj.position = this->position;
	//총알 위치 업데이트
	this->bullet_obj.position = this->position;
	//총알 방향 설정
	float yaw = glm::degrees(atan2(this->front.x, this->front.z));
	float pitch = glm::degrees(asin(this->front.y));
	this->bullet_obj.rotation.y = -glm::radians(yaw);
	this->bullet_obj.rotation.z = glm::radians(pitch);
}