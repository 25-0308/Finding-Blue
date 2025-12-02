#pragma once
#pragma once

#include "Object.h"

class INTRO {
private:
    glm::vec3 position;
    glm::vec3 front;
    bool logo_recoil = false;

public:
    Object E1;
    Object E2;
    Object E3;
    Object cigar;
    Object bench;
    Object tino;
    Object logo;
    Object guitar;

    INTRO()
        : E1("asset/guitar/enemy_guitar.obj", "asset/enemy/blue/blue_enemy.png"),
        E2("asset/enemy/sit/enemy_sit.obj", "asset/enemy/blue/blue_enemy.png"),
        E3("asset/enemy/sit/enemy_tabaco.obj", "asset/enemy/blue/blue_enemy.png"),
		cigar("asset/enemy/sit/cigar.obj", "asset/enemy/sit/cigar.png"),
		bench("asset/bench/bench.obj", "asset/bench/bench.png"),
		tino("asset/tino/tino_VIVA.obj", "asset/tino/tino_VIVA.png"),
        guitar("asset/guitar/guitar.obj", "asset/guitar/guitar.png"),
		logo("asset/logo/logo.obj", "asset/logo/logo.png")
    {
    }

    void init() {
        E1.init();
        E2.init();
		E3.init();
        cigar.init();
		bench.init();
		tino.init();
        logo.init();
		guitar.init();
        //회전
       cigar.rotation.y=guitar.rotation.y= E1.rotation.y=E2.rotation.y=E3.rotation.y=logo.rotation.y=bench.rotation.y=tino.rotation.y = glm::radians(270.0f);
        //
        bench.scale = glm::vec3(0.3f);
        bench.position = glm::vec3(0.0f, -0.5f, 1.0f);
		guitar.scale=E1.scale = E2.scale = E3.scale = glm::vec3(0.3f);
		logo.position= guitar.position=E1.position =   bench.position+glm::vec3(0.0,0.45,0.0);
		E2.position =   bench.position + glm::vec3(0.8, 0.45, 0.0);
		E3.position =   bench.position + glm::vec3(-0.8, 0.45, 0.0);
        cigar.position = E3.position;
		cigar.scale = glm::vec3(0.3f);
		
        logo.position.y += 1.0f;
        logo.position.z -= 0.7f;
		tino.scale = glm::vec3(0.5f);
        tino.position = E2.position + glm::vec3(0.0, 0.2, 0.4);
        
    }

    void draw(GLuint shader) {

     
        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 1.1f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);
      
		E1.draw(shader);
		E2.draw(shader);
		E3.draw(shader);
		cigar.draw(shader);
		bench.draw(shader);
		tino.draw(shader);
		logo.draw(shader);
		guitar.draw(shader);
        

    }
    //여기 아래로는 cpp 파일에 작성할 것들
    void update(float deltaTime);
    
   

};

