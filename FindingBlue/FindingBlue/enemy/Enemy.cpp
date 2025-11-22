#include"../Enemy.h"


void ENEMY::update() {
	//프라이빗의 position, front를 파츠들의 모델매트릭스에 적용
	head.position=body.position=Rarm1.position=Rarm2.position=Larm1.position =Larm2.position =Rleg1.position =Rleg2.position =Lleg1.position =Lleg2.position = this->position;

}