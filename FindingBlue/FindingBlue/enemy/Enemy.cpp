#include"../Enemy.h"
#include<iostream>
#include <random>
#include"../SoundManager.h"



bool ENEMY::update(float deltaTime, glm::vec3 target) {
	//거리에 따라 행동 다르게
	float distance = glm::length(target - this->position);
	
	//콜라이더 위치 업데이트
	collision.center = this->position;
	collision.center.y += 0.5f;
	if (!this->is_dead) {
		if (distance < 10.0f) {
			//달려옴
			this->run(deltaTime, target);
		}
		else if (distance >= 10.0f) {
			this->idle(deltaTime);
		}
	}
	else if (this->is_dead) {
		//사망모션
		if (!dead_sound) {
			static std::mt19937 rng(std::random_device{}()); // 한 번만 초기화
			std::uniform_int_distribution<int> dist(1, 5);
			int sound_index = dist(rng);
			if (sound_index == 1)
				SoundManager::Play("sin");
			else if (sound_index == 2)
				SoundManager::Play("bin");
			else if (sound_index == 3)
				SoundManager::Play("gun");
			else if (sound_index == 4)
				SoundManager::Play("yong");
			else if (sound_index == 5)
				SoundManager::Play("kark");
			dead_sound = true;
		}
		this->head.position.y += 15.5f * deltaTime;
		for (int i = 0; i < bloods.size(); i++) {
			bloods[i]->update(deltaTime);

			if (bloods[i]->is_dead()) {
				delete bloods[i];
				bloods.erase(bloods.begin() + i);
				i--;
			}
			if (bloods.empty()) {
				return true;
			}
		}
	}
	return false;
}

void ENEMY::run(float deltaTime,glm::vec3 target) {
	//포지션을 타겟을 향해 조금씩 이동
	glm::vec3 direction = glm::normalize(target - this->position);
	float speed = 2.0f; // 적의 이동 속도
	float temp = this->position.y;
	
	this->position += direction * speed * deltaTime;
	this->position.y = temp; // y축 고정
	
	
	
	//적이 플레이어를 바라보게 y축 각도 구하기
	glm::vec3 toTarget = glm::normalize(target - this->position);
	float desiredYaw = glm::degrees(atan2(toTarget.x, toTarget.z));
	head.rotation.y = body.rotation.y = Rarm1.rotation.y = Rarm2.rotation.y = Larm1.rotation.y = Larm2.rotation.y = Rleg1.rotation.y = Rleg2.rotation.y = Lleg1.rotation.y = Lleg2.rotation.y = glm::radians(desiredYaw-90.0f);


	//델타타임에 따라 회전
	float motion_angle =  glm::radians(200.0f * deltaTime); // 회전 속도 조절

	//팔의 첫번째
	if (this->Rarm1_rotate_dir) {
		
		Rarm1_rotate_angle += motion_angle;
		Rarm1.rotation.z = Rarm1_rotate_angle +glm::radians(180.0f);
		if (Rarm1_rotate_angle >= glm::radians(45.0f)) {
			this->Rarm1_rotate_dir = false;
		}
	}
	else {
	
		Rarm1_rotate_angle -= motion_angle;
		Rarm1.rotation.z = Rarm1_rotate_angle + glm::radians(180.0f);
		if (Rarm1_rotate_angle <= -glm::radians(45.0f)) {
			this->Rarm1_rotate_dir = true;
		}
	}
	if (this->Larm1_rotate_dir) {
	
		Larm1_rotate_angle += motion_angle;
		Larm1.rotation.z = Larm1_rotate_angle + glm::radians(180.0f);
		if (Larm1_rotate_angle >= glm::radians(45.0f)) {
			this->Larm1_rotate_dir = false;
		}
	}
	else {
		
		Larm1_rotate_angle -= motion_angle;
		Larm1.rotation.z = Larm1_rotate_angle + glm::radians(180.0f);
		if (Larm1_rotate_angle <= -glm::radians(45.0f)) {
			this->Larm1_rotate_dir = true;
		}
	}
	//위 각도들 360도 넘어가면 다시 줄여야함
	if (Rarm1.rotation.z > glm::radians(360.0f) || Rarm1.rotation.z < glm::radians(-360.0f)) {
		Rarm1.rotation.z = 0.0f;
	}
	if (Larm1.rotation.z > glm::radians(360.0f) || Larm1.rotation.z < glm::radians(-360.0f)) {
		Larm1.rotation.z = 0.0f;
	}

	
	//프라이빗의 position, front를 파츠들의 모델매트릭스에 적용
	head.position=body.position=Rarm1.position=Rarm2.position=Larm1.position =Larm2.position =Rleg1.position =Rleg2.position =Lleg1.position =Lleg2.position = this->position;

	//몸통y축회잔
	float B_angle = glm::radians(20.0f * deltaTime);

	if (this->body_bobbing_dir) {
		head.rotation.y = body.rotation.y = Rarm1.rotation.y = Rarm2.rotation.y = Larm1.rotation.y = Larm2.rotation.y = Rleg1.rotation.y = Rleg2.rotation.y = Lleg1.rotation.y = Lleg2.rotation.y  += B_angle;
		body_bobbing_angle += B_angle;
		if (body_bobbing_angle >= glm::radians(5.0f)) {
			this->body_bobbing_dir = false;
		}
	}
	else {
		head.rotation.y = body.rotation.y = Rarm1.rotation.y = Rarm2.rotation.y = Larm1.rotation.y = Larm2.rotation.y = Rleg1.rotation.y = Rleg2.rotation.y = Lleg1.rotation.y = Lleg2.rotation.y -= B_angle;
		body_bobbing_angle -= B_angle;
		if (body_bobbing_angle <= -glm::radians(5.0f)) {
			this->body_bobbing_dir = true;
		}
	}
	
	
	Rarm1.position.y += 0.5f * this->scale.y;
	Larm1.position.y += 0.5f * this->scale.y;
	//팔의 두번쨰
	glm::vec3 p1 = Rarm1.position;
	glm::vec3 p2 = Larm1.position;
	// Arm2가 Arm1 기준으로 떨어진 거리
	float length = 0.2f;

	


	glm::vec4 localOffset(0.0f, length, 0.0f, 1.0f);

	// Arm1의 회전
	glm::mat4 arm1Model =
		glm::translate(glm::mat4(1.0f), Rarm1.position) 
		* glm::rotate(glm::mat4(1.0f), body.rotation.y, glm::vec3(0, 1,0 ))
		* glm::rotate(glm::mat4(1.0f), Rarm1.rotation.z, glm::vec3(0, 0, 1));

	glm::vec4 worldPos = arm1Model * localOffset;
	Rarm2.position = glm::vec3(worldPos) + glm::vec3(0, 0.05f, 0);
	
	//왼팔회전
	glm::mat4 arm1Model_L =
		glm::translate(glm::mat4(1.0f), Larm1.position)
		* glm::rotate(glm::mat4(1.0f), body.rotation.y, glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), Larm1.rotation.z, glm::vec3(0, 0, 1));
	
	glm::vec4 worldPos_L = arm1Model_L * localOffset;
	Larm2.position = glm::vec3(worldPos_L) + glm::vec3(0, 0.05f, 0);




	motion_angle = glm::radians(150.0f * deltaTime); // 회전 속도 조절
	//다리부분
	//다리1
	float limit = glm::radians(45.0f);
	Rarm2.rotation.z = glm::clamp(Rarm1.rotation.z, -limit, limit);
	Larm2.rotation.z = glm::clamp(Larm1.rotation.z, -limit, limit);



	float leg_motion_angle = glm::radians(150.0f * deltaTime);

	// =========================
	// Right Leg 1
	// =========================
	if (this->Rleg1_rotate_dir)
	{
		Rleg1_rotate_angle += leg_motion_angle;
		Rleg1.rotation.z = Rleg1_rotate_angle + glm::radians(180.0f);

		if (Rleg1_rotate_angle >= glm::radians(30.0f))
			this->Rleg1_rotate_dir = false;
	}
	else
	{
		Rleg1_rotate_angle -= leg_motion_angle;
		Rleg1.rotation.z = Rleg1_rotate_angle + glm::radians(180.0f);

		if (Rleg1_rotate_angle <= -glm::radians(30.0f))
			this->Rleg1_rotate_dir = true;
	}

	//왼다리1
	if (this->Lleg1_rotate_dir)
	{
		Lleg1_rotate_angle += leg_motion_angle;
		Lleg1.rotation.z = Lleg1_rotate_angle + glm::radians(180.0f);

		if (Lleg1_rotate_angle >= glm::radians(30.0f))
			this->Lleg1_rotate_dir = false;
	}
	else
	{
		Lleg1_rotate_angle -= leg_motion_angle;
		Lleg1.rotation.z = Lleg1_rotate_angle + glm::radians(180.0f);

		if (Lleg1_rotate_angle <= -glm::radians(30.0f))
			this->Lleg1_rotate_dir = true;
	}



	if (Rleg1.rotation.z > glm::radians(360.0f) || Rleg1.rotation.z < glm::radians(-360.0f))
		Rleg1.rotation.z = 0.0f;

	if (Lleg1.rotation.z > glm::radians(360.0f) || Lleg1.rotation.z < glm::radians(-360.0f))
		Lleg1.rotation.z = 0.0f;
	Rleg1.position.y += 0.3f;
	Lleg1.position.y += 0.3f;
	//다리2

	localOffset=glm::vec4(0.07f, length, 0.0f, 1.0f);

	glm::mat4 leg1Model =
		glm::translate(glm::mat4(1.0f), Rleg1.position)
		* glm::rotate(glm::mat4(1.0f), body.rotation.y, glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), Rleg1.rotation.z, glm::vec3(0, 0, 1));

	 worldPos = leg1Model * localOffset;
	Rleg2.position = glm::vec3(worldPos) + glm::vec3(0, 0.05f, 0);
	Rleg2.position.y -= 0.05f;
	//왼다리회전
	glm::mat4 leg1Model_L =
		glm::translate(glm::mat4(1.0f), Lleg1.position)
		* glm::rotate(glm::mat4(1.0f), body.rotation.y, glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), Lleg1.rotation.z, glm::vec3(0, 0, 1));
	worldPos_L = leg1Model_L * localOffset;
	Lleg2.position = glm::vec3(worldPos_L) + glm::vec3(0, 0.05f, 0);
	Lleg2.position.y -= 0.05f;
	Rleg2.rotation.z =- glm::clamp(Rleg1.rotation.z, -limit, limit);
	Lleg2.rotation.z = -glm::clamp(Lleg1.rotation.z, -limit, limit);

}
void ENEMY::idle(float deltaTime)
{
	float speed = glm::radians(200.0f) * deltaTime;

	// ======== 회전 복구 함수 ========
	auto recoverTo = [&](float& rot, float target)
		{
			float diff = target - rot;

			if (glm::abs(diff) < speed)
				rot = target;
			else
				rot += glm::sign(diff) * speed;
		};

	
	recoverTo(Rarm1.rotation.z, glm::radians(180.0f));
	recoverTo(Larm1.rotation.z, glm::radians(180.0f));
	recoverTo(Rarm2.rotation.z, glm::radians(0.0f));
	recoverTo(Larm2.rotation.z, glm::radians(0.0f));

	recoverTo(Rleg1.rotation.z, glm::radians(180.0f));
	recoverTo(Lleg1.rotation.z, glm::radians(180.0f));

	//recoverTo(Rleg2.rotation.z, -glm::radians(0.0f));
	//recoverTo(Lleg2.rotation.z, -glm::radians(0.0f));

	Rleg2.rotation.z = -Rleg1.rotation.z;
	Lleg2.rotation.z = -Lleg1.rotation.z;
	// ==========================================
	//           Arm2 위치 계산 (Pivot)
	// ==========================================

	float length = 0.2f;
	glm::vec4 localOffset(0.0f, length, 0.0f, 1.0f);

	// ===== 오른팔 =====
	glm::mat4 Rarm1Model =
		glm::translate(glm::mat4(1.0f), Rarm1.position)
		* glm::rotate(glm::mat4(1.0f), body.rotation.y, glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), Rarm1.rotation.z, glm::vec3(0, 0, 1));

	glm::vec4 Rworld = Rarm1Model * localOffset;
	Rarm2.position = glm::vec3(Rworld) + glm::vec3(0, 0.05f, 0);

	// ===== 왼팔 =====
	glm::mat4 Larm1Model =
		glm::translate(glm::mat4(1.0f), Larm1.position)
		* glm::rotate(glm::mat4(1.0f), body.rotation.y, glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), Larm1.rotation.z, glm::vec3(0, 0, 1));

	glm::vec4 Lworld = Larm1Model * localOffset;
	Larm2.position = glm::vec3(Lworld) + glm::vec3(0, 0.05f, 0);


	// ==========================================
	//           Leg2 위치 계산 (Pivot)
	// ==========================================

	// 다리 길이 (필요시 조정)
	float legLength = 0.25f;
	glm::vec4 legOffset(0.0f, -legLength, 0.0f, 1.0f);

	// ===== 오른다리 =====
	glm::mat4 Rleg1Model =
		glm::translate(glm::mat4(1.0f), Rleg1.position)
		* glm::rotate(glm::mat4(1.0f), body.rotation.y, glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), Rleg1.rotation.z, glm::vec3(0, 0, 1));

	glm::vec4 RlegWorld = Rleg1Model * legOffset;
	Rleg2.position = glm::vec3(RlegWorld);

	// ===== 왼다리 =====
	glm::mat4 Lleg1Model =
		glm::translate(glm::mat4(1.0f), Lleg1.position)
		* glm::rotate(glm::mat4(1.0f), body.rotation.y, glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), Lleg1.rotation.z, glm::vec3(0, 0, 1));

	glm::vec4 LlegWorld = Lleg1Model * legOffset;
	Lleg2.position = glm::vec3(LlegWorld);
	glm::vec3 tempLR = Rleg2.position;
	glm::vec3 tempLL = Lleg2.position;
	Rleg2.position.y -=0.48;
	Lleg2.position.y -= 0.48;
	// ----------------------------------------
	// 최초 1회 초기 위치 보정
	// ----------------------------------------
	if (!this->initialized)
	{
		float offsetY = 0.5f * this->scale.y;

		Rarm1.position.y += offsetY;
		Larm1.position.y += offsetY;
		Rarm2.position.y += offsetY;
		Larm2.position.y += offsetY;

		Rleg1.position.y += offsetY*1.7;
		Lleg1.position.y += offsetY*1.7;
		Rleg2.position.y = tempLR.y + 10.0 * offsetY;
		Lleg2.position.y = tempLL.y + 10.0 * offsetY;

		this->initialized = true;
	}
}

glm::vec3 rotateAroundPivot(glm::vec3 point, glm::vec3 pivot, float angleZ)
{
	// pivot 기준으로 로컬 좌표 구함
	glm::vec3 local = point - pivot;

	// Z축 회전 행렬
	glm::mat3 rotZ(
		glm::vec3(cos(angleZ), sin(angleZ), 0.0f),
		glm::vec3(-sin(angleZ), cos(angleZ), 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	// 로컬 좌표 회전
	glm::vec3 rotatedLocal = rotZ * local;

	// 다시 pivot 기준으로 되돌림
	return pivot + rotatedLocal;
}
void ENEMY::take_damage(int damage) {
	this->health -= damage;
	std::cout << "ENEMY 체력: " << this->health << std::endl;
	if (this->health <= 0&&!this->is_dead) {
		is_dead = true;
		this->spawn_blood();
		std::cout << "ENEMY 사망!" << std::endl;
	}
}
float frand(float a, float b) {
	static std::mt19937 rng(std::random_device{}()); // 한 번만 초기화
	std::uniform_real_distribution<float> dist(a, b);
	return dist(rng);
}

void ENEMY::spawn_blood() {

	glm::vec3 headPos = this->position + glm::vec3(0, 0.5f, 0);

	const int count = 50;
	for (int i = 0; i < count; i++) {

		BLOOD* b = new BLOOD();

		// 각 파티클이 퍼질 방향
		glm::vec3 dir;

		// i에 따라 사분면 분배
		if (i < count/4*1) {             // 1사분면 (+x, +z)
			dir = glm::vec3(frand(0.5f, 1.0f), frand(0.2f, 1.0f), frand(0.5f, 1.0f));
		}
		else if (i < count / 4 * 2) {       // 2사분면 (-x, +z)
			dir = glm::vec3(frand(-1.0f, -0.5f), frand(0.2f, 1.0f), frand(0.5f, 1.0f));
		}
		else if (i < count / 4 * 3) {       // 3사분면 (-x, -z)
			dir = glm::vec3(frand(-1.0f, -0.5f), frand(0.2f, 1.0f), frand(-1.0f, -0.5f));
		}
		else {                  // 4사분면 (+x, -z)
			dir = glm::vec3(frand(0.5f, 1.0f), frand(0.2f, 1.0f), frand(-1.0f, -0.5f));
		}

		// 속도 조절
		dir = glm::normalize(dir) * frand(3.0f, 7.0f);

		b->init(headPos, dir);
		bloods.push_back(b);
	}
}
