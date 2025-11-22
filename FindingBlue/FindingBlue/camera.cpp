#include "Camera.h"

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
    return glm::lookAt(
        player->position,
        player->position + player->front,
        player->up
    );
}
