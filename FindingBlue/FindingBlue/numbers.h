#pragma once
#pragma once

#include "Object.h"

class NUMBER {
private:
    Object n0;
	Object n1;
	Object n2;
	Object n3;
	Object n4;
	Object n5;
	Object n6;
	Object n7;
	Object n8;
	Object n9;

    

public:
	std::vector<Object> digits;

    NUMBER():
		n0("asset/numbers/N0.obj", "asset/numbers/numbers.png"),
		n1("asset/numbers/N1.obj", "asset/numbers/numbers.png"),
		n2("asset/numbers/N2.obj", "asset/numbers/numbers.png"),
		n3("asset/numbers/N3.obj", "asset/numbers/numbers.png"),
		n4("asset/numbers/N4.obj", "asset/numbers/numbers.png"),
		n5("asset/numbers/N5.obj", "asset/numbers/numbers.png"),
		n6("asset/numbers/N6.obj", "asset/numbers/numbers.png"),
		n7("asset/numbers/N7.obj", "asset/numbers/numbers.png"),
		n8("asset/numbers/N8.obj", "asset/numbers/numbers.png"),
		n9("asset/numbers/N9.obj", "asset/numbers/numbers.png")

   

    {
    }

    void init();


    void draw(GLuint shader) {
   
        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 256.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 2.0f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 1.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);
       // airplane.draw(shader);
        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 0.001f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);
        


    }
    //여기 아래로는 cpp 파일에 작성할 것들
    void update(float deltaTime);
    void draw_number(GLuint shader, int number, glm::vec3 startPos);

};

