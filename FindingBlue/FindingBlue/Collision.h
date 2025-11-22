#pragma once
#include <GL/glm/glm.hpp>
#include <vector>
#include <memory>

class Collision 
{
public:
	enum Coll_Type {
		AABB,// 축정렬 바운딩 박스
		OBB // 기본 바운딩 박스
	};

	Collision(Coll_Type t) : type(t), isTrigger(false) {}
	virtual ~Collision() = default;

	Coll_Type getType() const { return type; } // 타입값 반환
	glm::vec3 getCenter() const { return center; } // 중심점 좌표 반환
	void SetCenter(const glm::vec3& c) { center = c; } // 중심점 좌표 설정

	bool IsTrigger() const { return isTrigger; } // 충돌 여부 반환
	void SetTrigger(bool trigger) { isTrigger = trigger; } // 충돌 여부 설정

	virtual bool Check_COLL(const Collision* other) const = 0; // 충돌 체크

	virtual void Debug_Draw() const = 0; // 바운딩박스 그리기

protected:
	Coll_Type type; // 바운딩 박스 타입
	glm::vec3 center; // 오브젝트 중심
	bool isTrigger = false; // 충돌 여부
};

namespace Debug_Draw {
	void Add_Line(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color = glm::vec3(1.0f,0.0f,0.0f)); // 빨간 선으로 
	void Render(); // 그리기
}