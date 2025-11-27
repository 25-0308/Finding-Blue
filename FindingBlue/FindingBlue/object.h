#pragma once
#include "ModelLoader.h"
#include "TextManager.h"

class Object {
public:
    RenderableObject obj;
    const char* objPath = nullptr;
    const char* texPath = nullptr;
    GLuint textureID = 0;
    glm::vec3 position = glm::vec3(0);
    glm::vec3 rotation = glm::vec3(0);
    glm::vec3 scale = glm::vec3(1.0);

    glm::vec3 pivot = glm::vec3(0.0f);
    


    Object() {}

    Object(const char* o, const char* t) {
        objPath = o;
        texPath = t;
    }

    void init() {

        if (objPath)
            CreateRenderableObject(objPath, obj); // 텍스처 포함시키지 않음

        if (texPath)
            textureID = TextureManager::GetTexture(texPath);
	
    }

    virtual void draw(GLuint shader) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glm::mat4 m = glm::mat4(1.0f);

        m = glm::translate(m, position);
     

        m = glm::rotate(m, rotation.y, glm::vec3(0, 1, 0));

        m = glm::translate(m, pivot);   // 1. pivot으로 이동
        m = glm::rotate(m, rotation.z, glm::vec3(0, 0, 1)); // 팔 회전
        m = glm::translate(m, -pivot);  // 3. pivot 복귀
        m = glm::scale(m, scale);
        m = glm::rotate(m, rotation.x, glm::vec3(1, 0, 0));
        obj.modelMatrix = m;

        drawObject(shader, obj);
    }


    virtual ~Object() {
        deleteObject(obj);
        // 텍스처는 삭제하지 않음 (공유 중)
    }
};
