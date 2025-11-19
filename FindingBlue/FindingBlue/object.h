#pragma once
#include "ModelLoader.h"


class Object {
public:
    RenderableObject obj;
    const char* objPath = nullptr;
    const char* texPath = nullptr;

    Object() {}

    Object(const char* o, const char* t) {
        objPath = o;
        texPath = t;
    }

    void init() {
        if (objPath && texPath)
            CreateRenderableObject(objPath, texPath, obj);
    }

    virtual void draw(GLuint shader) {
        drawObject(shader, obj);
    }

    virtual ~Object() {
        deleteObject(obj);
    }
};
