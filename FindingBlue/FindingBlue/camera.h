#pragma once
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include "Player.h"

class Camera {
public:
    Player* player;

    float yaw = -90.0f;
    float pitch = 0.0f;
    float sensitivity = 50.0f;
	float death_roll = 0.0f;
    Camera(Player& p) : player(&p) {}

    void updateDirection(float dx, float dy, float deltaTime);
    glm::mat4 getView() const;
	void death_view(float deltaTime);
};
