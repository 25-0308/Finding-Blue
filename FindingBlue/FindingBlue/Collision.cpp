#include "Collision.h"
#include <algorithm>
#include <cmath>
#include <vector>

namespace Debug_Draw {
	std::vector<glm::vec3> lines;
	void AddLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color = glm::vec3(1.0f, 0.0f, 0.0f) {
		lines.push_back(start);
		lines.push_back(end);
		lines.push_back(color);
	} // 선 추가
	void Render();
}

Collision::Collision(const glm::vec3 half) : halfsize(half){}

glm::vec3 Collision::Min() const {
	return center - halfsize;
} // 최소 좌표 반환
glm::vec3 Collision::Max() const {
	return center + halfsize;
}// 최대 좌표 반환

bool Collision::check_collision(const Collision& other) const {
	return (Max().x >= other.Min().x && Min().x <= other.Max().x) &&
		(Max().y >= other.Min().y && Min().y <= other.Max().y) &&
		(Max().z >= other.Min().z && Min().z <= other.Max().z);
} // 충돌 검사

void Collision::updateBox(const glm::vec3& localHalfsize,
	const glm::quat& rotation, const glm::vec3& position) {
	
	glm::mat3 rot = mat3_cast(rotation);

	glm::vec3 corners[8] = {
		rot * vec3(-localHalfSize.x, -localHalfSize.y, -localHalfSize.z),
		rot * vec3(localHalfSize.x, -localHalfSize.y, -localHalfSize.z),
		rot * vec3(localHalfSize.x,  localHalfSize.y, -localHalfSize.z),
		rot * vec3(-localHalfSize.x,  localHalfSize.y, -localHalfSize.z),
		rot * vec3(-localHalfSize.x, -localHalfSize.y,  localHalfSize.z),
		rot * vec3(localHalfSize.x, -localHalfSize.y,  localHalfSize.z),
		rot * vec3(localHalfSize.x,  localHalfSize.y,  localHalfSize.z),
		rot * vec3(-localHalfSize.x,  localHalfSize.y,  localHalfSize.z)
	};

	glm::vec3 minPoint = corners[0] + position;
	glm::vec3 maxPoint = corners[0] + position;

	for (int i = 1; i < 8; ++i) {
		glm::vec3 point = corners[i] + position;
		minPoint = glm::min(minPoint, worldCorner);
		maxPoint = glm::max(maxPoint, worldCorner);
	}

	center = (minPoint + maxPoint) * 0.5f;
	halfsize = (maxPoint - minPoint) * 0.5f;
} // 이동에 따른 업데이트

void Collision::Debug_Draw(const glm::vec3& color) const {
	glm::vec3 min = Min();
	glm::vec3 max = Max();

	glm::vec3 v[8] = {
		glm::vec3(min.x, min.y, min.z),
		glm::vec3(max.x, min.y, min.z),
		glm::vec3(max.x, max.y, min.z),
		glm::vec3(min.x, max.y, min.z),
		glm::vec3(min.x, min.y, max.z),
		glm::vec3(max.x, min.y, max.z),
		glm::vec3(max.x, max.y, max.z),
		glm::vec3(min.x, max.y, max.z)
	};

	const int edges[24] = {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};
	for (int i = 0; i < 24; i += 2) {
		Debug_Draw::AddLine(v[edges[i]], v[edges[i + 1]], color);
	}
} // 빨간 박스 그리기