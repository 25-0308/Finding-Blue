#pragma once
#include "Object.h"
#include"weapon.h"
#include"bullet.h"
class MINIGUN : public Weapon {
private:
    //총이 가지고 있어야하는 그런것들임
    glm::vec3 position;
    glm::vec3 front;
    
    bool is_get = false;
    bool recoil_mode = false;
	float head_rotate_angle_offset = 0.0f;
	float rotate_speed = 0.0f;

public:
    //총알 목록들
    std::vector<BULLET*> bullets;
    Object head;
    Object body;
    glm::vec3 offsets = glm::vec3(0.0f);
    glm::vec3 for_bullet_offset = glm::vec3(0.0f);
    MINIGUN()
        : head("asset/minigun/minigun_head.obj", "asset/minigun/minigun.png"),
        body("asset/minigun/minigun_body.obj", "asset/minigun/minigun.png")
    {
    }

    void init() override {
        head.init();
        body.init();
        head.scale = body.scale = glm::vec3(0.3f);
        head.position = body.position = glm::vec3(45.0f, -0.2f, 45.0f);
    }

    void draw(GLuint shader)override {

        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 256.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 10.0f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 1.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 2.5f);
		head.draw(shader);
        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 8.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 0.2f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);
        body.draw(shader);


        //총알이 안비어있다면 드로우
        if (!bullets.empty()) {
            for (auto& b : bullets) {
                b->draw(shader);
            }

        }

    }
    //여기 아래로는 cpp 파일에 작성할 것들
    void update(float deltaTime, glm::vec3 position, float yaw, float pitch)override;
    bool get_weapon(glm::vec3 playerPos);
    bool get_is_get() {
        return is_get;
    }
    void attack(float deltaTime) override; //총 반동도 여기 넣을ㄹ거

    bool get_recoil_mode() { return this->recoil_mode; };
    void set_recoil_mode(bool mode) { this->recoil_mode = mode; };
    void zoom_in(bool mode, float deltaTime)override;

};
BULLET* shoot_bullet(glm::vec3 postion, glm::vec3 direction);
