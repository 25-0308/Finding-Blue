#pragma once
//--- 프로그램 맨 앞에 선언할 것

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>
#include <random>
#include <string>
#include <GL/glm/glm.hpp>
#include <GL/glm/ext.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
struct Vec3 { float x, y, z; };
struct Vec2 { float u, v; };

struct Face {
    unsigned int v[3];
    unsigned int t[3];
    unsigned int n[3];
};

struct Model {
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Vec2> texcoords;
    std::vector<Face> faces;
};

struct RenderableObject {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint textureID = 0;
    size_t vertexCount = 0;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
};

// OBJ + Texture 로딩 API
bool CreateRenderableObject(
    const char* objPath,
    const char* texPath,
    RenderableObject& outObj
);
void drawObject(GLuint shader, RenderableObject& obj);
// GPU에서 제거할 때 쓰기
void deleteObject(RenderableObject& obj);
