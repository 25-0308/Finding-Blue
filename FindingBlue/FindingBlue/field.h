#pragma once
#include "Object.h"
#include "Collision.h"

class FIELD {
   

public:
    Object tile;
    Object wall;
    Object sky;
    std::vector<Object> tiles;
	std::vector<Object> walls;
	int opening_walls_idx[2] = { -1,-1 }; //�� ������ Ÿ�� �ε��� �����


	std::vector<Collision> collisions;
    Collision button_wall;

    FIELD();
    void init() {
		tiles.reserve(100);
		walls.reserve(100);

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
		for (auto& c : collisions)
			c.Debug_Draw(glm::vec3(0.0f, 1.0f, 0.0f));
    }
};
