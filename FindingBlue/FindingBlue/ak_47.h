#pragma once

#include "Object.h"


class AK_47 {
public:
    Object wood;
    Object metal;

    AK_47() :
        wood("asset/ak_47/ak_47_wood.obj", "asset/ak_47/ak_v2.png"),
        metal("asset/ak_47/ak_47_metal.obj", "asset/ak_47/ak_v2.png")
    {
    }

    void init() {
        wood.init();
        metal.init();
    }

    void draw(GLuint shader) {
        wood.draw(shader);
        metal.draw(shader);
    }
};
