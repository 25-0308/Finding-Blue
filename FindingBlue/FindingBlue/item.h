#pragma once
#include "Object.h"
enum class ItemType {
    Meat,
    RottenMeat,
    Ammo
};

static const char* getModelPath(ItemType t);
static const char* getTexturePath(ItemType t);
class ITEM  {
private:
    
    glm::vec3 position;
    glm::vec3 front;

    bool is_get = false;



public:
    //총알 목록들
    Object item;
    ItemType type;

    ITEM(ItemType t)
        : type(t),
        item(getModelPath(t), getTexturePath(t))
    {
    }

    void init(glm::vec3 pos) {
        item.init();
        item.scale = glm::vec3(0.2f);
	
        item.position = pos;
        item.position.y -= 0.5f;
        if (type == ItemType::Ammo) {
            item.scale = glm::vec3(0.7f);
			item.position.y -= 0.5f;
        }
		std::cout << "아이템 타입" << "" << static_cast<int>(type) << std::endl;
    }

    void draw(GLuint shader) {

        
        glUniform1f(glGetUniformLocation(shader, "material.shininess"), 8.0f);
        glUniform1f(glGetUniformLocation(shader, "material.specularStrength"), 0.2f);
        glUniform1f(glGetUniformLocation(shader, "material.metallic"), 0.0f);
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), 1.0f);

        item.draw(shader);

        

    }
    //여기 아래로는 cpp 파일에 작성할 것들
   
    bool get_is_get() {
        return is_get;
    }

    bool get_item(glm::vec3 playerPos) {
        float distance = glm::length(playerPos - this->item.position);
        if (distance < 1.5f && !this->is_get) {
            this->is_get = true;
            return true;
        }
        return false;

    }

};
static const char* getModelPath(ItemType t) {
    switch (t) {
    case ItemType::Meat:       return "asset/item/meat.obj";
    case ItemType::RottenMeat: return "asset/item/meat.obj";
    case ItemType::Ammo:       return "asset/item/ammos.obj";
    }
    return "";
}

static const char* getTexturePath(ItemType t) {
    switch (t) {
    case ItemType::Meat:       return "asset/item/meat.png";
    case ItemType::RottenMeat: return "asset/item/rottenmeat.png";
    case ItemType::Ammo:       return "asset/item/ammo.png";
    }
    return "";
}
