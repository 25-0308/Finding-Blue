
#pragma once

#include "Object.h"

class CLEARLOGO {
private:

    glm::vec3 position;

    float missile_cooldown = 1.0f;
    int shoot_count = 0;
    bool is_active = false;

public:
    Object Clogo;

    CLEARLOGO()
        : Clogo("asset/logo/clearlogo.obj", "asset/logo/clearlogo.png")
      


    {
    }

    void init() {
        Clogo.init();
		Clogo.scale = glm::vec3(0.0f);
		Clogo.position = glm::vec3(0.0, 0.0, 0.0f);
		Clogo.rotation.y = glm::radians(270.0f);

    }

    void draw(GLuint shader) {
       
      
        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 0.001f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);
		Clogo.draw(shader);


    }
    void update(float deltaTime) {
		//로고 확대
        if (Clogo.scale.x < 1.5f) {
            Clogo.scale += glm::vec3(1.5f * deltaTime);
            //회전

            Clogo.rotation.z += glm::radians(5000.0f) * deltaTime;
			if (Clogo.rotation.z > glm::radians(360.0f)) {
				Clogo.rotation.z -= glm::radians(360.0f);
			}
        }

    }
    



};

