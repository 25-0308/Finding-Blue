#include"../pistol.h"


void PISTOL::update(float deltaTime, glm::vec3 position, float yaw, float pitch)
{
    if (!this->is_get) {
        head.rotation.y += glm::radians(60.0f) * deltaTime;
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
        this->front = front;
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
        glm::vec3 up = glm::normalize(glm::cross(right, front));


        glm::vec3 offset = right * (0.1f + this->offsets.x)   // 화면 오른쪽으로
            + up * (-0.1f + this->offsets.y)    // 화면 아래로
            + front * (0.3f ); // 화면 안쪽으로

        glm::vec3 offset_head = right * (0.1f + this->offsets.x)   // 화면 오른쪽으로
            + up * (-0.1f + this->offsets.y)    // 화면 아래로
            + front * (0.3f + this->offsets.z); // 화면 안쪽으로

        glm::vec3 gunPos = position + offset;
		glm::vec3 gunPos_head = position + offset_head;
        this->for_bullet_offset = gunPos;
        this->for_bullet_offset.y += 0.1f;
        // 총 위치 설정
        this->head.position = gunPos_head;
        this->metal.position = gunPos;
        //방향설정
        head.rotation.y = -glm::radians(yaw);
        metal.rotation.y = -glm::radians(yaw);
        head.rotation.z = glm::radians(pitch);
        metal.rotation.z = glm::radians(pitch);
    }

    for (int i = bullets.size() - 1; i >= 0; --i)
    {
        BULLET* b = bullets[i];


        if (b->update(deltaTime, yaw, pitch)) // 삭제 조건 만족?
        {
            delete b;            // 메모리 해제
            bullets.erase(bullets.begin() + i);  // 벡터에서 제거
        }
    }

}

bool PISTOL::get_weapon(glm::vec3 playerPos) {
    float distance = glm::length(playerPos - this->head.position);
    if (distance < 1.5f && !this->is_get) {
        this->is_get = true;
        return true;
    }
    return false;

}

void PISTOL::attack(float deltaTime) {
    //총 반동 구현
    if (!this->recoil_mode) {
        //총 오프셋 뒤로
        this->offsets.z += 2.0f * deltaTime;
        if (this->offsets.z > 0.0) {
            this->recoil_mode = true;
            this->offsets.z = 0.0f;
            //이 부분에 총알 생성 넣으면 될거같다
            this->on_attak = false;
            this->shoot_cooldown = 1.0f;
            
       
        }
    }
    else if (this->recoil_mode) {
        //총 오프셋 앞으로
        this->offsets.z -= 2.0f * deltaTime;
        if (this->offsets.z < -0.1f) {
            this->recoil_mode = false;
            this->offsets.z = -0.1f;
            bullets.push_back(shoot_bullet(this->for_bullet_offset, this->front));
            this->ammo -= 1;
        }
    }
}

void PISTOL::zoom_in(bool mode, float deltaTime) {
    //조준모드 구현
    if (mode) {
        //오프셋 y값 줄이기
        offsets.x -= 0.1f * deltaTime;
        if (offsets.x) {
            offsets.x < -0.1f;
            offsets.x = -0.1f;
        }
    }
    else if (!mode) {
        //오프셋 y값 늘리기
        offsets.x += 1.0f * deltaTime;
        if (offsets.x > 0.0f) {
            offsets.x = 0.0f;
        }
    }
}

void PISTOL::reload() {
	this->ammo = 10;
}