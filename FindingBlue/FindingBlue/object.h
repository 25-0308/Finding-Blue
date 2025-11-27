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

    glm::vec3 pivot = glm::vec3(0.0f);
    
	Collision collision;
	glm::vec3 localHalfsize = glm::vec3(0.5f);

    Object() {}

    Object(const char* o, const char* t) {
        objPath = o;
        texPath = t;
    }

    void updateBox() {
		collision.updateBox(localHalfsize, position, scale);
    }; // ?¥Îèô???∞Î•∏ ?ÖÎç∞?¥Ìä∏



    void init() {

        if (objPath)
            CreateRenderableObject(objPath, obj); // ?çÏä§Ï≤??¨Ìï®?úÌÇ§ÏßÄ ?äÏùå

        if (texPath)
            textureID = TextureManager::GetTexture(texPath);

		updateBox();
    }

    virtual void draw(GLuint shader) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glm::mat4 m = glm::mat4(1.0f);

        m = glm::translate(m, position);
     

        m = glm::rotate(m, rotation.y, glm::vec3(0, 1, 0));
        m = glm::translate(m, pivot);  //1. pivot¿∏∑Œ ¿Ãµø
        m = glm::rotate(m, rotation.z, glm::vec3(0, 0, 1)); // ∆»»∏¿¸
        m = glm::translate(m, -pivot);  // 3. pivot ∫π±Õ
        m = glm::scale(m, scale);
        m = glm::rotate(m, rotation.x, glm::vec3(1, 0, 0));
        obj.modelMatrix = m;

        drawObject(shader, obj);

		updateBox();

    	collision.Debug_Draw(glm::vec3(1, 0, 0)); // Îπ®Í∞Ñ ??Ï∂îÍ? Í∑∏Î¶¨Í∏?
    }


    virtual ~Object() {
        deleteObject(obj);
        // ?çÏä§Ï≤òÎäî ??†ú?òÏ? ?äÏùå (Í≥µÏú† Ï§?
    }
};
