#include"../claymore.h"


void CLAYMORE::update(float deltaTime, glm::vec3 position, float yaw, float pitch)
{
    if (!this->is_get) {
		
		metal.rotation.y += glm::radians(60.0f) * deltaTime;
    }
    else if (this->is_get) {
        this->position = position;
        this->front = front;


        // 카메라 기준 front 벡터 계산
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);

        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
        glm::vec3 up = glm::normalize(glm::cross(right, front));


        glm::vec3 offset = right * 0.2f   // 화면 오른쪽으로
            + up * -0.7f    // 화면 아래로
            + front * 0.8f; // 화면 안쪽으로
  

        glm::vec3 gunPos = position + offset;

        // 총 위치 설정
        this->wood.position = gunPos;
        this->metal.position = gunPos;
        //방향설정
        wood.rotation.y = -glm::radians(yaw)+glm::radians(90.0f);
        metal.rotation.y = -glm::radians(yaw) + glm::radians(90.0f);
        wood.rotation.z = glm::radians(pitch);
        metal.rotation.z = glm::radians(pitch);

        //오프셋더함
        wood.rotation.y += this->attack_offsets.y;
		metal.rotation.y += this->attack_offsets.y;

        wood.rotation.z += this->attack_offsets.z;
		metal.rotation.z += this->attack_offsets.z;
    }

}

bool CLAYMORE::get_weapon(glm::vec3 playerPos) {
	float distance = glm::length(playerPos - this->metal.position);
	if (distance < 1.5f&&!this->is_get) {
		this->is_get = true;
        return true;
	}
    return false;
}

void CLAYMORE::attack(float deltaTime) {
    //이건 x축에대해 회전적용할거임
    if (!this->recoil_mode) {
        //총 오프셋 뒤로
        //총 오프셋 앞으로
        this->attack_offsets.z -= glm::radians(800.0f * deltaTime);
	
        if (this->attack_offsets.z < glm::radians(-90.0f)) {
            this->recoil_mode = true;
            this->attack_offsets.z = glm::radians(-90.0f);
   

        }
    }
    else if (this->recoil_mode) {
       
        this->attack_offsets.z += glm::radians(200.0f * deltaTime);
    
        if (this->attack_offsets.z > glm::radians(0.0f)) {
            this->recoil_mode = false;
            this->attack_offsets.z = glm::radians(0.0f);
            this->on_attak = false;
    
        }
    }
   metal.rotation.z=wood.rotation.z = this->attack_offsets.z;
}
void CLAYMORE::zoom_in(bool mode, float deltaTime) {
	//빠따는 줌인모드 없음
}