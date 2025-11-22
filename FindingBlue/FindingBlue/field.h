#pragma once
#include "Object.h"
class FIELD {
   

public:
    Object tile;

    std::vector<Object> tiles;

    FIELD() {
        const char* obj = "asset/tile/tile.obj";
        const char* tex = "asset/tile/tile.png";

        int W = 20;  // 가로 타일 수
        int H = 20;  // 세로 타일 수
        float tileSize = 5.0;

        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {

                Object t(obj, tex);

                t.position = glm::vec3(x * tileSize, -1.0f, y * tileSize);
                t.scale = glm::vec3(0.5f);

                tiles.push_back(t);
            }
        }
    }

    void init() {
        for (auto& t : tiles)
            t.init();
    }

    void draw(GLuint shader) {
        for (auto& t : tiles)
            t.draw(shader);
    }
};
