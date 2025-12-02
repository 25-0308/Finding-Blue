#include"../airplane.h"


void AIRPLANE::update(float deltaTime)
{
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
}


