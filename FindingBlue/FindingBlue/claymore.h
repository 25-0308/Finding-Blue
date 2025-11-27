#pragma once
#pragma once

#include "Object.h"
#include"weapon.h"
class CLAYMORE : public Weapon {
private:
    //클레이모어가 가지고 있어야하는 그런것들임
    glm::vec3 position;
    glm::vec3 front;
    bool is_get = false;
    bool recoil_mode = false;
    glm::vec3 attack_offsets = glm::vec3(0.0f);

public:
    Object wood;
    Object metal;

    CLAYMORE()
        : wood("asset/claymore/claymore_wood.obj", "asset/claymore/claymore_v1.png"),
        metal("asset/claymore/claymore_metal.obj", "asset/claymore/claymore_v1.png")
    {
    }

    void init() {
        wood.init();
        metal.init();
        wood.scale = metal.scale = glm::vec3(0.3f);
        wood.position=metal.position = glm::vec3(25.0f, -0.5f,20.0f);
        attack_offsets.y = glm::radians(270.0f);
    }

    void draw(GLuint shader) {

        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 256.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 2.0f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 1.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);
        metal.draw(shader);
        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 0.001f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);
		wood.draw(shader);
     

    }
    //여기 아래로는 cpp 파일에 작성할 것들
    void update(float deltaTime, glm::vec3 position, float yaw, float pitch);
    bool get_weapon(glm::vec3 playerPos);
    bool get_is_get() {
        return is_get;
    }
    void attack(float deltaTime) override;
    bool get_recoil_mode() { return this->recoil_mode; };
    void set_recoil_mode(bool mode) { this->recoil_mode = mode; };
    void zoom_in(bool mode, float deltaTime)override;

};

