#pragma once

#include "Object.h"
#include"weapon.h"
class CLUB : public Weapon {
private:
    //빠따 가지고 있어야하는 그런것들임
    glm::vec3 position;
    glm::vec3 front;
	bool is_get = false;


public:
    Object hand;
    Object metal;

    CLUB()
        : hand("asset/bbadda/hand.obj", "asset/bbadda/club.png"),
        metal("asset/bbadda/bbadda.obj", "asset/bbadda/club.png")
    {
    }

    void init() {
        hand.init();
        metal.init();
        hand.scale = metal.scale = glm::vec3(0.1f);
		metal.position = glm::vec3(15.0f, -0.2f, 10.0f);
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

            if (this->is_get)
                hand.draw(shader);
        
    }
    //여기 아래로는 cpp 파일에 작성할 것들
    void update(float deltaTime, glm::vec3 position, float yaw, float pitch);
	bool get_weapon(glm::vec3 playerPos);
    bool get_is_get() {
        return is_get;
    }
	void attack(float deltaTime) override {
		//빠따 휘두르기 구현
	}
    void zoom_in(bool mode, float deltaTime)override;

};

