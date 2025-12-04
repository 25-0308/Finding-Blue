#include"../club.h"
#include <iostream>

void CLUB::update(float deltaTime, glm::vec3 position, float yaw, float pitch)
{
    if (!this->is_get) {
        metal.rotation.y += glm::radians(60.0f) * deltaTime;
        hit_active = false;
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


        glm::vec3 offset = right * 0.4f   // 화면 오른쪽으로
            + up * -0.7f    // 화면 아래로
            + front * 0.8f; // 화면 안쪽으로


        glm::vec3 gunPos = position + offset;

        // 총 위치 설정
        this->hand.position = gunPos;
        this->metal.position = gunPos;
        //방향설정
        hand.rotation.y = -glm::radians(yaw) + glm::radians(90.0f);
        metal.rotation.y = -glm::radians(yaw) + glm::radians(90.0f);
        hand.rotation.z = glm::radians(pitch);
        metal.rotation.z = glm::radians(pitch);

        //오프셋더함
        hand.rotation.y += this->attack_offsets.y;
        metal.rotation.y += this->attack_offsets.y;

        hand.rotation.z += this->attack_offsets.z;
        metal.rotation.z += this->attack_offsets.z;


    }
    if (hit_active) {
        update_hit_collider();
    }
}

bool CLUB::get_weapon(glm::vec3 playerPos) {
    float distance = glm::length(playerPos - this->metal.position);
    if (distance < 1.5f && !this->is_get) {
        this->is_get = true;
        return true;
    }
    return false;
}

void CLUB::attack(float deltaTime) {
    if (!this->recoil_mode) {
        this->attack_offsets.z -= glm::radians(800.0f * deltaTime);

        // 각도에 들어왔을 때 콜라이더 생성 (한 번만)
        if (is_in_hit_angle() && !hit_trigger) {
            create_hit_collider();
            hit_trigger = true;
        }

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

            // 공격 종료 시 콜라이더 제거
            hit_active = false;
            hit_trigger = false;
        }
    }

    metal.rotation.z = hand.rotation.z = this->attack_offsets.z;
}

void CLUB::zoom_in(bool mode, float deltaTime) {
    //빠따는 줌인모드 없음
}

bool CLUB::is_in_hit_angle() const {
    // 스윙 각도가 -30도 ~ -60도 사이일 때 히트 가능
    float swing_angle = glm::degrees(this->attack_offsets.z);
    return (swing_angle >= -60.0f && swing_angle <= -30.0f);
}

glm::vec3 CLUB::get_club_tip_position() const {
    if (!this->is_get) return glm::vec3(0);

    // 클럽에 맞게 위치조정
    glm::vec3 club_tip_offset = glm::vec3(0.0f, 0.5f, -0.45f);

    // 클럽의 변환 행렬 생성
    glm::mat4 clubTransform = glm::mat4(1.0f);
    clubTransform = glm::translate(clubTransform, this->metal.position);
    clubTransform = glm::rotate(clubTransform, this->metal.rotation.y, glm::vec3(0, 1, 0));
    clubTransform = glm::rotate(clubTransform, this->metal.rotation.z, glm::vec3(0, 0, 1));

    // 클럽 끝 위치 계산
    glm::vec4 tip_world = clubTransform * glm::vec4(club_tip_offset, 1.0f);
    return glm::vec3(tip_world);
}

void CLUB::create_hit_collider() { // 사이즈 조절도 여기서
    // 이미 활성화되어 있으면 생성하지 않음
    if (hit_active) return;

    // 클럽 끝부분에 히트 콜라이더 생성
    glm::vec3 tip_pos = get_club_tip_position();

    collision.center = tip_pos;
    collision.halfsize = glm::vec3(0.2f, 0.2f, 0.3f);
    hit_active = true;

    std::cout << "클럽 콜라이더 생성!" << std::endl;
}

void CLUB::update_hit_collider() {
    if (hit_active) {
        // 히트 콜라이더 위치 업데이트
        collision.center = get_club_tip_position();
    }
}
void CLUB::reload() {
	
}