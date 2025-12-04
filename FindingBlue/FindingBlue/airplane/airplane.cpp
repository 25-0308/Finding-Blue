#include"../airplane.h"


void AIRPLANE::update(float deltaTime)
{
	if (!this->is_active) { 
		
		return; }
  //z좌표 증가
	airplane.position.z += 40.0f * deltaTime;
	end_cyclone.rotation.z=start_cyclone.rotation.z += glm::radians(90.0f) * deltaTime;


	//시작 블랙홀,사이크론은 비행기의 좌표가 20이하일 때 커지고 그 이상은 작아져야함
	if (airplane.position.z < 20.0f) {
		start_blackhole.scale = start_cyclone.scale += glm::vec3(5.5f) * deltaTime;
		if (start_blackhole.scale.x >= 13.0f) {
			start_blackhole.scale = start_cyclone.scale = glm::vec3(13.0f);
		}
	}
	else {
		start_blackhole.scale = start_cyclone.scale -= glm::vec3(10.5f) * deltaTime;
		if (start_blackhole.scale.x <= 0.0f) {
			start_blackhole.scale = start_cyclone.scale = glm::vec3(0.0f);
		}
		//그리고 끝 블랙홀,사이크론은 여기서부터 증가
		end_blackhole.scale = end_cyclone.scale += glm::vec3(5.5f) * deltaTime;
		if (end_blackhole.scale.x >= 13.0f) {
			end_blackhole.scale = end_cyclone.scale = glm::vec3(13.0f);
		}
	}
	//비행기가끝지점 지나면 끝블랙홀,사이클론 감소
	if (airplane.position.z > 120.0f) {
		end_blackhole.scale = end_cyclone.scale -= glm::vec3(10.5f) * deltaTime;
		if (end_blackhole.scale.x <= 0.0f) {
			end_blackhole.scale = end_cyclone.scale = glm::vec3(0.0f);
			
		}
	}

	//
	if (airplane.position.z >= 100.0f) {
		this->is_active = false;
	}
}


bool AIRPLANE::missile_ready(float deltaTime) {
	//비행기가 특정 좌표 지날때 미사일 발사준비
	if (airplane.position.z > 0.0f &&airplane.position.z<=90.0f) {
		missile_cooldown += deltaTime;
		if (missile_cooldown > 1.0f) {
			missile_cooldown = 0.0f;
			return true;
		}
	}
	return false;
}

int AIRPLANE::get_airplane_state() {
	//0이하 준비단계
	if (airplane.position.z < 15.0f)
		return 0;
	//0~120 비행단계
	else if (airplane.position.z >= 15.0f && airplane.position.z <= 120.0f)
		return 1;
	//120이상 종료단계
	else
		return 2;
}
void AIRPLANE::is_player_in_range(glm::vec3 playerPos) {
	//맵 안에 있는 경우 4545,7575사이
	if (!this->is_active) {
		if (playerPos.x > 43.0f && playerPos.x < 88.0f && playerPos.z > 43.0f && playerPos.z < 88.0f) {
			if (airplane.position.z < 15.0f)
			this->set_active(true);
		}

	}
	
}