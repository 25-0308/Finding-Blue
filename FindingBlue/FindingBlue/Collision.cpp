#include "Collision.h"
#include <algorithm>
#include <vector>
#include <array>
#include <GL/glm/gtc/type_ptr.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>



namespace Debug_Draw {
	std::vector<glm::vec3> lines;

	void AddLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color) {
		lines.push_back(start);
		lines.push_back(end);
		lines.push_back(color);
	}
	void Render( ) {

		
		glDisable(GL_TEXTURE_2D);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		for (size_t i = 0; i + 2 < lines.size(); i += 3) {
			glColor3fv(glm::value_ptr(lines[i + 2]));
			glVertex3fv(glm::value_ptr(lines[i]));
			glVertex3fv(glm::value_ptr(lines[i + 1]));
		}
		glEnd();
		glLineWidth(1.0f);
		glEnable(GL_TEXTURE_2D);
		lines.clear();
	}
}

Collision::Collision(const glm::vec3 half) : halfsize(half) {}

glm::vec3 Collision::Min() const {
    return center - halfsize;
}

glm::vec3 Collision::Max() const {
    return center + halfsize;
}


bool Collision::check_collision(const Collision& other) const {
	return (Max().x >= other.Min().x && Min().x <= other.Max().x) &&
		(Max().y >= other.Min().y && Min().y <= other.Max().y) &&
		(Max().z >= other.Min().z && Min().z <= other.Max().z);
} // 충돌 검사

void Collision::Transform(const glm::vec3& position, const glm::vec3& rotation)
{
	// 원본 AABB의 8개 꼭짓점 계산
	glm::vec3 original_vertices[8] = {
		glm::vec3(-halfsize.x, -halfsize.y, -halfsize.z),
		glm::vec3(halfsize.x, -halfsize.y, -halfsize.z),
		glm::vec3(halfsize.x,  halfsize.y, -halfsize.z),
		glm::vec3(-halfsize.x,  halfsize.y, -halfsize.z),
		glm::vec3(-halfsize.x, -halfsize.y,  halfsize.z),
		glm::vec3(halfsize.x, -halfsize.y,  halfsize.z),
		glm::vec3(halfsize.x,  halfsize.y,  halfsize.z),
		glm::vec3(-halfsize.x,  halfsize.y,  halfsize.z)
	};

	// 회전 행렬 생성
	glm::mat4 rotMatrix = glm::mat4(1.0f);
	rotMatrix = glm::rotate(rotMatrix, rotation.y, glm::vec3(0, 1, 0)); // Yaw
	rotMatrix = glm::rotate(rotMatrix, rotation.x, glm::vec3(1, 0, 0)); // Pitch  
	rotMatrix = glm::rotate(rotMatrix, rotation.z, glm::vec3(0, 0, 1)); // Roll

	// 회전된 꼭짓점들 계산
	glm::vec3 rotated_vertices[8];
	for (int i = 0; i < 8; i++) {
		rotated_vertices[i] = glm::vec3(rotMatrix * glm::vec4(original_vertices[i], 1.0f));
	}

	// 회전된 꼭짓점들을 포함하는 새로운 AABB 계산
	glm::vec3 min_bounds = rotated_vertices[0];
	glm::vec3 max_bounds = rotated_vertices[0];

	for (int i = 1; i < 8; i++) {
		min_bounds = glm::min(min_bounds, rotated_vertices[i]);
		max_bounds = glm::max(max_bounds, rotated_vertices[i]);
	}

	// 새로운 center와 halfsize 설정
	center = position + (min_bounds + max_bounds);
	halfsize = (max_bounds - min_bounds);
}

void Collision::Debug_Draw(const glm::vec3& color) const {

	glm::vec3 fixedHalf = halfsize;  

	glm::vec3 min = center - fixedHalf;
	glm::vec3 max = center + fixedHalf;

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
		0,1, 1,2, 2,3, 3,0,
		4,5, 5,6, 6,7, 7,4,
		0,4, 1,5, 2,6, 3,7
	};

	for (int i = 0; i < 24; i += 2) {
		Debug_Draw::AddLine(v[edges[i]], v[edges[i + 1]], color);
	}
}

