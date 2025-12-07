#include"../missile.h"
#include"../SoundManager.h"


bool MISSILE::update(float deltaTime)
{
	explosion_effect.position.y=missile.position.y -= 5.0f * deltaTime;
	missile.rotation.z -= glm::radians(10.0f) * deltaTime;
	//바닥과 도착시
	if (missile.position.y < 0.0f && !exploded) {
		exploded = true;
		SoundManager::Play("boom", 0.7f);
	}

	if (this->exploded && !this->explosion_decrease) {
		explosion_effect.scale += glm::vec3(30.0f) * deltaTime;
		explosion_effect.rotation.y += glm::radians(180.0f) * deltaTime;
		//일정 크기 도달 시 감소
		if (this->explosion_effect.scale.x > 40.0f) {
			this->explosion_decrease = true;
		}
	}
	else if (this->explosion_decrease) {
		explosion_effect.scale -= glm::vec3(60.0f) * deltaTime;
		explosion_effect.rotation.y += glm::radians(180.0f) * deltaTime;
		if (this->explosion_effect.scale.x < 0.0f) {
			//삭제코드
			return true;
		}
	
	}
	return false;
}


