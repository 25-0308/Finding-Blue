#pragma once
#include"object.h"
class Weapon {
public:
    glm::vec3 position = glm::vec3(0);
    glm::vec3 rotation = glm::vec3(0);

    virtual void init() {}
    virtual void update(float dt, glm::vec3 playerPos, float yaw, float pitch) {}
    virtual void draw(GLuint shader) {}
	virtual void attack() {}
	virtual void zoom_in(bool mode, float deltaTime) {}

    virtual ~Weapon() {}
};
