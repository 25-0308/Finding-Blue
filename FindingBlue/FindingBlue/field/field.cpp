#include"../field.h"
#include<iostream>

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
    //여기서부터는 맵을 구성하는 벽들임충돌처리 적용해야함
    //오프셋
    Collision temp;
    temp.halfsize = glm::vec3(0.5f, 5.0f, 2.5f);

    glm::vec3 offset = glm::vec3(-2.5f, 0.0f, -2.5f);
    Object wall1("asset/tile/wall.obj", "asset/tile/wall.png");
    wall1.scale = glm::vec3(0.5f);
    wall1.position = glm::vec3(tileSize * 1.0, -0.75f, 0.0) + offset;
    wall1.rotation.y = glm::radians(90.0f);
    walls.push_back(wall1);
    //벽1 충돌처리
    temp.center = wall1.position;
    collisions.push_back(temp);

    //벽2
    wall1.position = glm::vec3(tileSize * 1.0, -0.75f, tileSize * 1.0) + offset;
    walls.push_back(wall1);
    //벽2 충돌처리
    temp.center = wall1.position;
    collisions.push_back(temp);

    //벽3
    wall1.position = glm::vec3(tileSize * 1.0, -0.75f, tileSize * 2.0) + offset;
    walls.push_back(wall1);
    //벽3 충돌처리
    temp.center = wall1.position;
    collisions.push_back(temp);

    //버튼과 상호작용 하는 벽(맨처음부분)
    wall1.position = glm::vec3(tileSize * 0.05, -0.75f, 2.0 * tileSize);
    wall1.rotation.y = glm::radians(0.0f);
    walls.push_back(wall1);
    
    //벽4 충돌처리
    temp.center = wall1.position;
    temp.halfsize = glm::vec3(2.5f, 5.0f, 0.5f);
    collisions.push_back(temp);
    collision_wall_idx[0] = collisions.size() - 1;
    



    std::cout << "벽 pos" << wall1.position.x << " " << wall1.position.y << " " << wall1.position.z << std::endl;
    this->opening_walls_idx[0] = walls.size() - 1;
    //중앙에 작게 네모난벽 3x3크기 
// 중앙 3x3 작은 방 생성
    int minX = 6, maxX = 11;
    int minY = 6, maxY = 11;

    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            // 테두리가 아니면 패스 (즉, 안쪽 빈 공간은 벽을 만들지 X)
            if (!(x == minX || x == maxX || y == minY || y == maxY))
                continue;

            // 기본 벽 생성
            Object w("asset/tile/wall.obj", "asset/tile/wall.png");
            w.scale = glm::vec3(0.5f);
            w.position = glm::vec3(x * tileSize, -0.75f, y * tileSize) + offset;

            // ──────────────────────────────
            //      방향별 회전 적용
            // ──────────────────────────────
            if (x == minX) {                 // 왼쪽 벽
                w.rotation.y = glm::radians(90.0f);
                w.position.x -= 2.5f;
                temp.halfsize = glm::vec3(0.5f, 5.0f, 2.5f);
            }
            else if (x == maxX) {            // 오른쪽 벽
                w.rotation.y = glm::radians(-90.0f);
                w.position.x += 2.5f;
                temp.halfsize = glm::vec3(0.5f, 5.0f, 2.5f);
                
            }
            else if (y == minY) {            // 아래쪽 벽
                w.rotation.y = glm::radians(180.0f);
                w.position.z -= 2.5f;
                temp.halfsize = glm::vec3(2.5f, 5.0f, 0.5f);
            }
            else {                           // 위쪽 벽 (y == maxY)
                w.rotation.y = glm::radians(0.0f);
                w.position.z += 2.5f;
                temp.halfsize = glm::vec3(2.5f, 5.0f, 0.5f);
            }
			
            walls.push_back(w);
            temp.center = w.position;
            collisions.push_back(temp);
            //이 부분 벽은 버튼열림
            if (y == minY+2&&x==maxX) {
                this->opening_walls_idx[1] = walls.size() - 1;
                collision_wall_idx[1] = collisions.size()-1;
                //디버깅출력
                std::cout << "벽 개수" << walls.size() << std::endl;
                std::cout << "콜라이더" << collisions.size() << std::endl;
            }
            // ──────────────────────────────
            //       모서리 → 벽 하나 더
            // ──────────────────────────────
            bool isCorner =
                (x == minX || x == maxX) &&
                (y == minY || y == maxY);

            if (isCorner) {
                Object w2("asset/tile/wall.obj", "asset/tile/wall.png");
                w2.scale = glm::vec3(0.5f);
                w2.position = glm::vec3(x * tileSize, -0.75f, y * tileSize) + offset;

                // corner orientation
                if (x == minX && y == minY) {              // 좌하
                    w2.rotation.y = glm::radians(180.0f);
                    w2.position.z -= 2.5f;
                    temp.halfsize = glm::vec3(2.5f, 5.0f, 0.5f);
                }
                else if (x == minX && y == maxY) {         // 좌상
                    w2.rotation.y = glm::radians(0.0f);
                    w2.position.z += 2.5f;
                    temp.halfsize = glm::vec3(2.5f, 5.0f, 0.5f);
                }
                else if (x == maxX && y == minY) {         // 우하
                    w2.rotation.y = glm::radians(180.0f);
                    w2.position.z -= 2.5f;
                    temp.halfsize = glm::vec3(2.5f, 5.0f, 0.5f);
                }
                else if (x == maxX && y == maxY) {         // 우상
                    w2.rotation.y = glm::radians(0.0f);
                    w2.position.z += 2.5f;
                    temp.halfsize = glm::vec3(2.5f, 5.0f, 0.5f);
                }
                temp.center = w2.position;
                collisions.push_back(temp);
                walls.push_back(w2);
            }
            
        }
    }



    //하늘 임시로 해봄
	sky = Object("asset/sky/sky_smooth.obj", "asset/sky/sky.png");
	sky.obj.modelMatrix = glm::mat4(1.0f);
	sky.position = glm::vec3(47.5f, 0.0f, 47.5f);
	sky.scale = glm::vec3(1.1f);


}

void FIELD::update(float deltaTime, int idx) {
   
	int idx2 = collision_wall_idx[idx];
	idx = opening_walls_idx[idx];
    if (walls[idx].position.y > -10.0f) {
        walls[idx].position.y -= 2.0f * deltaTime;
        collisions[idx2].center = walls[idx].position;
    }
}