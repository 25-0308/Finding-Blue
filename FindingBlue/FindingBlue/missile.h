#pragma once
#pragma once

#include "Object.h"

class MISSILE {
private:

    glm::vec3 position;
	bool exploded = false;
	bool explosion_decrease = false;

public:
    Object missile;
	Object explosion_effect;

    MISSILE()
        : missile("asset/airplane/missile.obj", "asset/airplane/missile.png"),
		explosion_effect("asset/airplane/cyclone_sphere.obj", "asset/airplane/cyclone_sphere.png")
        


    {
    }

    void init(glm::vec3 pos) {
        missile.init();
        explosion_effect.init();
       
        missile.scale = glm::vec3(1.0f);
        missile.scale.x = 1.3f;
		missile.rotation.y = glm::radians(270.0f);
        explosion_effect.scale = glm::vec3(0.2f);
        explosion_effect.position=missile.position = pos;


    }

    void draw(GLuint shader) {

        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 256.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 2.0f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 1.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);
        missile.draw(shader);
        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 0.001f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.8f);
        explosion_effect.draw(shader);
        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 0.001f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);


    }
    //여기 아래로는 cpp 파일에 작성할 것들
    bool update(float deltaTime);


};

