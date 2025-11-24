#pragma once
#include "Object.h"
#include"weapon.h"
class AK_47 : public Weapon {
private:
    //총이 가지고 있어야하는 그런것들임
	glm::vec3 position;
	glm::vec3 front;    

    bool is_get = false;
	bool recoil_mode = false;


public:
    Object wood;
    Object metal;
    glm::vec3 offsets = glm::vec3(0.0f);
    AK_47()
        : wood("asset/ak_47/ak_47_wood.obj", "asset/ak_47/ak_v2.png"),
        metal("asset/ak_47/ak_47_metal.obj", "asset/ak_47/ak_v2.png")
    {
    }

    void init() override {
        wood.init();
        metal.init();
		wood.scale = metal.scale = glm::vec3(0.3f);
		wood.position = metal.position = glm::vec3(5.0f, -0.2f, 10.0f);
    }

    void draw(GLuint shader)override {

            glUniform1f(glGetUniformLocation(shader, "material.shininess"), 256.0f);
            glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 10.0f);
            glUniform1f(glGetUniformLocation(shader, "material.metallic"), 1.0f);
            glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 2.5f);
            metal.draw(shader);
            glUniform1f(glGetUniformLocation(shader, "material.shininess"), 8.0f);
            glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 0.2f);
            glUniform1f(glGetUniformLocation(shader, "material.metallic"), 0.0f);
            glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);

            wood.draw(shader);
        
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

