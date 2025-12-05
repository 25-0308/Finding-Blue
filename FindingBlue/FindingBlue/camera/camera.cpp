#include "../Camera.h"

void Camera::updateDirection(float dx, float dy, float deltaTime)
{
    float sens = sensitivity * deltaTime;

    yaw += dx * sens;
    pitch -= dy * sens;

    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    player->front = glm::normalize(front);
}

glm::mat4 Camera::getView() const
{
	glm::vec3 p_up = player->up;
	p_up = glm::rotate(glm::mat4(1.0f), glm::radians(death_roll), player->front) * glm::vec4(p_up, 0.0f);
    return glm::lookAt(
        player->position,
        player->position + player->front,
        p_up
    );
}
void Camera::death_view(float deltaTime) {
    if (player->position.y >-0.3f) {
		player->position.y -= 2.0f * deltaTime;
      
    }
    if (death_roll < 30.0f) {
        death_roll += 50.0f * deltaTime;
    }
}
