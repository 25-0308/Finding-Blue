#include"../field.h"


FIELD::FIELD() {
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
            //벽만드는거 일단 가장자리만
            if (x == 0 || x == W - 1 || y == 0 || y == H - 1) {

                // 기본 벽 하나 생성
                Object w("asset/tile/wall.obj", "asset/tile/wall.png");
                w.position = glm::vec3(x * tileSize, -0.75f, y * tileSize);
                w.scale = glm::vec3(0.5f);

                // 벽 방향 설정
                if (x == 0) {// 왼쪽 벽
                    w.rotation.y = glm::radians(90.0f);
                    w.position.x -= 2.5f; // 벽이 타일 밖으로 약간 나가도록 조정
                }
                else if (x == W - 1)
                {   // 오른쪽 벽
                    w.rotation.y = glm::radians(-90.0f);
                    w.position.x += 2.5f; // 벽이 타일 밖으로 약간 나가도록 조정
                }
                else if (y == 0)
                {   // 아래쪽 벽
                    w.rotation.y = glm::radians(180.0f);
                    w.position.z -= 2.5f; // 벽이 타일 밖으로 약간 나가도록 조정
                }
                else
                {  // 위쪽 벽
                    w.rotation.y = glm::radians(0.0f);
                    w.position.z += 2.5f; // 벽이 타일 밖으로 약간 나가도록 조정
                }

                walls.push_back(w);
				
                //  모서리면 벽 하나 더
                if ((x == 0 || x == W - 1) && (y == 0 || y == H - 1)) {

                    Object w2("asset/tile/wall.obj", "asset/tile/wall.png");
                    w2.position = glm::vec3(x * tileSize, -0.75f, y * tileSize);
                    w2.scale = glm::vec3(0.5f);

                    // 두 번째 벽의 방향은 y축 기준 반대 방향으로 세팅
                    if (x == 0 && y == 0)
                    {
                        w2.rotation.y = glm::radians(180.0f); // 위쪽
                        w2.position.z -= 2.5f; // 벽이 타일 밖으로 약간 나가도록 조정
                    }
                    else if (x == 0 && y == H - 1) {
                        w2.rotation.y = glm::radians(0.0f);
                        w2.position.z += 2.5f; // 벽이 타일 밖으로 약간 나가도록 조정
                    }
                    else if (x == W - 1 && y == 0) {
                        w2.rotation.y = glm::radians(180.0f);
                        w2.position.z -= 2.5f; // 벽이 타일 밖으로 약간 나가도록 조정
                    }
                    else if (x == W - 1 && y == H - 1) {
                        w2.rotation.y = glm::radians(0.0f);
                        w2.position.z += 2.5f; // 벽이 타일 밖으로 약간 나가도록 조정
                    }

                    walls.push_back(w2);
                }
            }

        }
    }
    //하늘 임시로 해봄
	sky = Object("asset/sky/sky_smooth.obj", "asset/sky/sky.png");
	sky.obj.modelMatrix = glm::mat4(1.0f);
	sky.position = glm::vec3(47.5f, 0.0f, 47.5f);
	sky.scale = glm::vec3(1.1f);


}