#pragma once
#pragma once
#include <GL/glm/glm.hpp>

class Player {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    bool keys[256] = { false };

    float speed = 5.0f;

    Player() :
        position(0.0f, 0.0f, 5.0f),
        front(0.0f, 0.0f, -1.0f),
        up(0.0f, 1.0f, 0.0f)
    {
    }

    void move(float deltaTime);
};
