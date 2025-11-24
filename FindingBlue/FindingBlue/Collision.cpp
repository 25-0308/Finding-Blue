#include "Collision.h"
#include <algorithm>
#include <vector>
#include <array>
#include <GL/glm/gtc/type_ptr.hpp>




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

void Collision::updateBox(const glm::vec3& localHalfsize, const glm::vec3& position, const glm::vec3& scale)
{
	glm::vec3 scaledHalf = localHalfsize * scale;
	center = position;
	halfsize = glm::vec3(1.0,2.0,2.0);

}

void Collision::Debug_Draw(const glm::vec3& color) const {

	// 항상 같은 크기의 박스를 사용 (원하면 값 바꿔도 됨)
	glm::vec3 fixedHalf = glm::vec3(0.5f);  // → 박스 크기 1x1x1

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
	//std::cout <<"Cpos"<< center.x << "," << center.y << "," << center.z << std::endl;
	//std::cout <<"Bpos" << v[0].x << "," << v[0].y << "," << v[0].z << std::endl;
	//std::cout<<"---------------" << std::endl;
}
// 빨간 박스 그리기