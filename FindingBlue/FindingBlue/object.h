#pragma once
#include "ModelLoader.h"
#include "TextManager.h"

class Object {
public:
    RenderableObject obj;
    const char* objPath = nullptr;
    const char* texPath = nullptr;
    GLuint textureID = 0;

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

        drawObject(shader, obj);
    }

    virtual ~Object() {
        deleteObject(obj);
        // 텍스처는 삭제하지 않음 (공유 중)
    }
};
