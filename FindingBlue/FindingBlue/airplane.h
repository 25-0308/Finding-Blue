#pragma once
#pragma once

#include "Object.h"

class AIRPLANE  {
private:

    glm::vec3 position;

	float missile_cooldown = 1.0f;
	int shoot_count = 0;
    bool is_active = false;

public:
    Object airplane;
    Object start_blackhole;
    Object start_cyclone;
	Object end_blackhole;
	Object end_cyclone;

    AIRPLANE()
        : airplane("asset/airplane/airplane.obj", "asset/airplane/airplane.png"),
		start_blackhole("asset/airplane/blackhole.obj", "asset/airplane/blackhole.png"),
		start_cyclone("asset/airplane/cyclone.obj", "asset/airplane/cyclone.png"),
		end_blackhole("asset/airplane/blackhole.obj", "asset/airplane/blackhole.png"),
		end_cyclone("asset/airplane/cyclone.obj", "asset/airplane/cyclone.png")

   
    {
    }

    void init() {
        airplane.init();
		start_blackhole.init();
		start_cyclone.init();
		end_blackhole.init();
		end_cyclone.init();
      airplane.scale = glm::vec3(1.0f);
      airplane.position = glm::vec3(45.0, 20.0, -100.0f);
	  airplane.rotation.y = glm::radians(270.0f);
	  //시작블,사
      start_blackhole.scale = start_cyclone.scale = glm::vec3(1.0f);
      start_blackhole.position = start_cyclone.position = glm::vec3(45.0, 20.0, -4.8);

	  //끝블,사
	  end_blackhole.scale = end_cyclone.scale = glm::vec3(0.0f);
	  end_blackhole.rotation.y = end_cyclone.rotation.y = glm::radians(180.0f);
	  end_blackhole.position = end_cyclone.position = glm::vec3(45.0, 20.0, 101.0);

    }

    void draw(GLuint shader) {
        if (!this->is_active) return;
        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 256.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 2.0f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 1.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);
        airplane.draw(shader);
        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 0.001f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);
		start_blackhole.draw(shader);
		start_cyclone.draw(shader);
		end_blackhole.draw(shader);
		end_cyclone.draw(shader);
        

    }
    //여기 아래로는 cpp 파일에 작성할 것들
    void update(float deltaTime);
	bool missile_ready(float deltaTime);
    glm::vec3 get_position() {
        return this->airplane.position;
    }
    int get_airplane_state();
	void set_active(bool active) {
		this->is_active = active;
	}
	bool get_is_active() {
		return this->is_active;
	}
	void is_player_in_range(glm::vec3 playerPos);
    

   
};

