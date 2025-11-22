#include "Player.h"
#include <GL/glm/gtc/matrix_transform.hpp>

void Player::move(float deltaTime)
{
    glm::vec3 forward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    glm::vec3 right = glm::normalize(glm::cross(forward, up));

    if (keys['w']) position += forward * speed * deltaTime;
    if (keys['s']) position -= forward * speed * deltaTime;
    if (keys['a']) position -= right * speed * deltaTime;
    if (keys['d']) position += right * speed * deltaTime;
}
