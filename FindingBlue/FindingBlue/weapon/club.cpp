#include"../club.h"


void CLUB::update(float deltaTime, glm::vec3 position, float yaw, float pitch)
{
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


    glm::vec3 offset = right * 0.5f   // 화면 오른쪽으로
        + up * -0.3f    // 화면 아래로
        + front * 0.8f; // 화면 안쪽으로

    glm::vec3 gunPos = position + offset;

    // 총 위치 설정
    this->hand.position = gunPos;
    this->metal.position = gunPos;
    //방향설정
    hand.rotation.y = -glm::radians(yaw);
    metal.rotation.y = -glm::radians(yaw);
	hand.rotation.z = glm::radians(pitch);
	metal.rotation.z = glm::radians(pitch);



}

