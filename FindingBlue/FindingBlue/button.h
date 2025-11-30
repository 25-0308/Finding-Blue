#pragma once
#include "Object.h"


class BUTTON {
private:

    glm::vec3 position;
    glm::vec3 front;

    bool is_hit = false;



public:
    //총알 목록들
    Object button;
    

    BUTTON()
        : button("asset/button/button.obj", "asset/button/button.png")
    {
    }

    void init(glm::vec3 pos,float rad) {
        button.init();
        button.scale = glm::vec3(1.2f);
        button.rotation.y = rad;
        button.position = pos;
		std::cout << "버튼 위치" << button.position.x << " " << button.position.y << " " << button.position.z << std::endl;
      
    }

    void draw(GLuint shader) {


        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 8.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 0.2f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);

        button.draw(shader);



    }
    //여기 아래로는 cpp 파일에 작성할 것들

    bool get_is_get() {
        return is_hit;
    }


};
