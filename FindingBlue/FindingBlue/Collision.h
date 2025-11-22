#pragma once
#include <GL/glm/glm.hpp>
#include <Gl/glm/gtc/quaternion.hpp>

class Collision 
{
public:
	glm::vec3 center = glm::vec3(0.0f); // 중심점 좌표
	glm::vec3 halfsize = glm::vec3(1.0f, 1.0f, 1.0f); // 절반 사이즈

	Collision() = default;
	explicit Collision(const glm::vec3 half); 

	glm::vec3 Min() const; // 최소 좌표 반환
	glm::vec3 Max() const; // 최대 좌표 반환

	bool check_collision(const Collision& other) const; // 충돌 검사

	void updateBox(const glm::vec3& localHalfsize, 
		const glm::quat& rotation, const glm::vec3& position = glm::vec3(0)); // 이동에 따른 업데이트

	void Debug_Draw(const glm::vec3& color = glm::vec3(1.0f, 0.0f, 0.0f)) const; // 빨간 박스 그리기
};
