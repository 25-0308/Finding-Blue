#include"../intro.h"



void INTRO::update(float deltaTime) {
	logo.rotation.y += glm::radians(30.0f) * deltaTime;
	if (logo.rotation.y > glm::radians(360.0f))
		logo.rotation.y = 0.0f;
	if (!this->logo_recoil) {
		//스케일증가
		logo.scale += glm::vec3(0.4f) * deltaTime;
		if (logo.scale.x >= 1.2f) {
			logo.scale = glm::vec3(1.2f);
			this->logo_recoil = true;
		}
	}
	else {
		//스케일감소
		logo.scale -= glm::vec3(0.4f) * deltaTime;
		if (logo.scale.x <= 0.5f) {
			logo.scale = glm::vec3(0.5f);
			this->logo_recoil = false;
		}
	}
}
