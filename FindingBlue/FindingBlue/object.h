#pragma once
#include "ModelLoader.h"
#include "TextManager.h"
#include "Collision.h"

class Object {
public:
    RenderableObject obj;
    const char* objPath = nullptr;
    const char* texPath = nullptr;
    GLuint textureID = 0;

    glm::vec3 position = glm::vec3(0);
    glm::vec3 rotation = glm::vec3(0);
    glm::vec3 scale = glm::vec3(1.0);
   
    Collision collision;
	glm::vec3 localHalfsize = glm::vec3(0.5f); // 1x1x1 기본 사이즈

    Object() {}

    Object(const char* o, const char* t) {
        objPath = o;
        texPath = t;
    }

    void updateBox() {
		glm::vec3 rot = glm::radians(rotation); // 라디안으로 각도 변환
		glm::quat quat = glm::quat(rot); // 변한 각도에 따라 회전행렬 생성

		glm::vec3 scaledHalfsize = localHalfsize * scale; // 스케일 적용

		collision.updateBox(scaledHalfsize, quat, position);
    }; // 이동에 따른 업데이트



    void init() {

        if (objPath)
            CreateRenderableObject(objPath, obj); // 텍스처 포함시키지 않음

        if (texPath)
            textureID = TextureManager::GetTexture(texPath);

		updateBox();
    }

    virtual void draw(GLuint shader) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glm::mat4 m = glm::mat4(1.0f);
        m = glm::rotate(m, rotation.y, glm::vec3(0, 1, 0));   // Yaw (y-axis)
        m = glm::rotate(m, rotation.x, glm::vec3(1, 0, 0));   // Pitch (x-axis)
        m = glm::rotate(m, rotation.z, glm::vec3(0, 0, 1));   // Roll (z-axis)
        obj.modelMatrix =
            glm::translate(glm::mat4(1.0f), position) *
			m *
            glm::scale(glm::mat4(1.0f), scale);
        drawObject(shader, obj);

		updateBox();

    	collision.Debug_Draw(glm::vec3(1, 0, 0)); // 빨간 선 추가 그리기
    }

    virtual ~Object() {
        deleteObject(obj);
        // 텍스처는 삭제하지 않음 (공유 중)
    }
};
