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
        item.scale = glm::vec3(0.1f);
        item.position = pos;
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
