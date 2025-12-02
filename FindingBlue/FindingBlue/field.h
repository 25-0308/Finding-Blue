#pragma once
#include "Object.h"





class FIELD {
   

public:
    Object tile;
    Object wall;
    Object sky;
    std::vector<Object> tiles;
	std::vector<Object> walls;
	int opening_walls_idx[2] = { -1,-1 }; //벽 열어줄 타일 인덱스 저장용
    
    FIELD();
    void init() {
        tiles.reserve(300);
		walls.reserve(300);
        for (auto& t : tiles)
            t.init();
		for (auto& w : walls)
			w.init();
		sky.init();
    }

    void draw(GLuint shader) {
        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 0.001f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);
      
        for (auto& t : tiles)
            t.draw(shader);
		for (auto& w : walls)
			w.draw(shader);
		sky.draw(shader);
    }
};
