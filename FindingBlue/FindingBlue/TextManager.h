#pragma once
#include <string>
#include <unordered_map>
#include <gl/glew.h>

GLuint LoadTexture(const char* path); // 기존 로더 그대로 사용

class TextureManager {
private:
    static std::unordered_map<std::string, GLuint> texCache;

public:
    static GLuint GetTexture(const std::string& path) {
        // 이미 로드된 경우
        if (texCache.find(path) != texCache.end())
            return texCache[path];

        // 새로 로드
        GLuint tex = LoadTexture(path.c_str());
        texCache[path] = tex;
        return tex;
    }

    static void Clear() {
        for (auto& t : texCache)
            glDeleteTextures(1, &t.second);
        texCache.clear();
    }
};
