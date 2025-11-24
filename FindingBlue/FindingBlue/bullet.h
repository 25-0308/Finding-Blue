#pragma once
#include"object.h"


class BULLET {
private:
    //총이 가지고 있어야하는 그런것들임
    glm::vec3 position;
    glm::vec3 front;



public:
  
    Object bullet_obj;
    glm::vec3 offsets = glm::vec3(0.0f);
    BULLET()
        : bullet_obj("asset/ammo/bullet.obj", "asset/ammo/bullet.png")
    {
    }

    void init()  {
      
        bullet_obj.init();
        bullet_obj.scale = glm::vec3(0.1f);
        bullet_obj.position = glm::vec3(5.0f, -0.2f, 10.0f);
    }

    void draw(GLuint shader) {

        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 256.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 10.0f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 1.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 2.5f);
        bullet_obj.draw(shader);
        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 8.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 0.2f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);


    }
    //여기 아래로는 cpp 파일에 작성할 것들
    void update(float deltaTime);
    void set_position(glm::vec3 pos) {
        this->position = bullet_obj.position =pos;
    }
	glm::vec3 get_position() {
		return this->position;
	}
	void set_front(glm::vec3 front) {
		this->front = front;
	}
    glm::vec3 get_front() {
		return this->front;
    }

};

