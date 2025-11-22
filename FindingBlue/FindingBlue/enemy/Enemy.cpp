#include"../Enemy.h"


void ENEMY::update(float deltaTime,glm::vec3 target) {
	//포지션을 타겟을 향해 조금씩 이동
	glm::vec3 direction = glm::normalize(target - this->position);
	float speed = 2.0f; // 적의 이동 속도
	/*this->position += direction * speed * deltaTime;*/
	//Rarm1.rotation.z += 0.001f;




	//프라이빗의 position, front를 파츠들의 모델매트릭스에 적용
	head.position=body.position=Rarm1.position=Rarm2.position=Larm1.position =Larm2.position =Rleg1.position =Rleg2.position =Lleg1.position =Lleg2.position = this->position;

}