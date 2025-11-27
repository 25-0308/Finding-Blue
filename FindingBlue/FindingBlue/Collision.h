#pragma once
#include <GL/glm/glm.hpp>
#include <Gl/glm/gtc/quaternion.hpp>
#include <GL/glut.h>


namespace Debug_Draw {
	void AddLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color = glm::vec3(1.0f, 0.0f, 0.0f));
	void Render(  );
}
class Collision
{
public:
	glm::vec3 center = glm::vec3(0.0f); // 중심점 좌표
	glm::vec3 halfsize = glm::vec3(1.0f); // 절반 사이즈

	Collision() = default;
	explicit Collision(const glm::vec3 half);

	glm::vec3 Min() const; // 최소 좌표 반환
	glm::vec3 Max() const; // 최대 좌표 반환

	bool check_collision(const Collision& other) const; // 충돌 검사

	void Debug_Draw(const glm::vec3& color = glm::vec3(1.0f, 0.0f, 0.0f)) const; // 빨간 박스 그리기
};