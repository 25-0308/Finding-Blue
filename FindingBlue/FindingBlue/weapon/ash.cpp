#include"../ash.h"


bool ASH::update(float deltaTime, float yaw, float pitch) {
	//총알 이동
	float speed = 10.0f; // 총알 속도
	this->position += this->front * speed * deltaTime;
	plane.position = this->position;





	//회전
	//fire.pivot = this->position;

	//
	plane.scale += 2.0f * deltaTime;
	if (plane.scale.x > 5.0f)
		plane.scale = glm::vec3(5.0f);
	//조명강도감소
	this->intensity -= 20.1f * deltaTime;
	if (this->intensity < 1.0f)
		this->intensity = 1.0f;

	//총알 위치 업데이트
	this->plane.position = this->position;

	//알파감소
	this->alpha -= 0.5f * deltaTime;
	if (this->alpha < 0.2f)
		//this->alpha = 0.0f;
		return true;
	//총알 방향 설정
	if (this->initialized == false) {
		this->plane.rotation.y = -glm::radians(yaw);
		this->plane.rotation.z = glm::radians(pitch);
		this->initialized = true;
	}

	//총알 콜라이더 업데이트
	collision.center = this->position;

	//총알이 맵밖 150범위 벗어나면 삭제
	if (glm::length(this->position) > 150.0f) {
		return true;
	}


	return false;
}