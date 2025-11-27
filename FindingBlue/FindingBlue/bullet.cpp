#include"bullet.h"


bool BULLET::update(float deltaTime, float yaw, float pitch) {
	//ÃÑ¾Ë ÀÌµ¿
	float speed = 100.0f; // ÃÑ¾Ë ¼Óµµ
	this->position += this->front * speed * deltaTime;
	bullet_obj.position = this->position;
	//ÃÑ¾Ë À§Ä¡ ¾÷µ¥ÀÌÆ®
	this->bullet_obj.position = this->position;
	//ÃÑ¾Ë ¹æÇâ ¼³Á¤
	if (this->initialized == false) {
		this->bullet_obj.rotation.y = -glm::radians(yaw);
		this->bullet_obj.rotation.z = glm::radians(pitch);
		this->initialized = true;
	}
	//ÃÑ¾ËÀÌ ¸Ê¹Û 150¹üÀ§ ¹ş¾î³ª¸é »èÁ¦
	if (glm::length(this->position) > 150.0f) {
		return true;
	}
	return false;
}