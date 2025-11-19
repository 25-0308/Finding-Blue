#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ModelLoader.h"
const int MAX_LINE_LENGTH = 10000;


void read_newline(char* str) {
	char* pos;
	if ((pos = strchr(str, '\n')) != NULL)
		*pos = '\0';
}
void read_newline_b(char* str) {
	size_t len = strlen(str);
	if (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
		str[len - 1] = '\0';
}

bool LoadOBJ(const char* filename, Model& model) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("Failed to open OBJ\n");
		return false;
	}

	char line[256];

	while (fgets(line, sizeof(line), file)) {

		// ---- Vertex ----
		if (strncmp(line, "v ", 2) == 0) {
			Vec3 v;
			sscanf(line + 2, "%f %f %f", &v.x, &v.y, &v.z);
			model.vertices.push_back(v);
		}

		// ---- Texcoord ----
		else if (strncmp(line, "vt ", 3) == 0) {
			Vec2 t;
			sscanf(line + 3, "%f %f", &t.u, &t.v);

			// Blender ↔ OpenGL 맞추기 위해 V를 반전
			t.v = 1.0f - t.v;

			model.texcoords.push_back(t);
		}

		// ---- Normal ----
		else if (strncmp(line, "vn ", 3) == 0) {
			Vec3 n;
			sscanf(line + 3, "%f %f %f", &n.x, &n.y, &n.z);
			model.normals.push_back(n);
		}

		// ---- Face ----
		else if (strncmp(line, "f ", 2) == 0) {
			Face f;

			int v[3] = { 0 }, t[3] = { 0 }, n[3] = { 0 };

			// 다양한 OBJ face 포맷 지원
			int count = sscanf(
				line + 2,
				"%d/%d/%d %d/%d/%d %d/%d/%d",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2]
			);

			if (count != 9) {
				// v/t or v//n 등 예외처리
				count = sscanf(
					line + 2,
					"%d/%d %d/%d %d/%d",
					&v[0], &t[0],
					&v[1], &t[1],
					&v[2], &t[2]
				);

				if (count == 6) {
					n[0] = n[1] = n[2] = 0; // 없으니까 0으로
				}
				else {
					count = sscanf(
						line + 2,
						"%d//%d %d//%d %d//%d",
						&v[0], &n[0],
						&v[1], &n[1],
						&v[2], &n[2]
					);
					t[0] = t[1] = t[2] = 0;
				}
			}

			for (int i = 0; i < 3; i++) {
				f.v[i] = v[i] - 1;
				f.t[i] = t[i] - 1;
				f.n[i] = n[i] - 1;
			}

			model.faces.push_back(f);
		}
	}

	fclose(file);
	return true;
}

struct GPUVertex {
	float px, py, pz;
	float nx, ny, nz;
	float u, v;
};
std::vector<GPUVertex> BuildMesh(const Model& model) {
	std::vector<GPUVertex> out;

	for (const auto& f : model.faces) {
		for (int i = 0; i < 3; i++) {
			GPUVertex g;

			Vec3 v = model.vertices[f.v[i]];
			Vec3 n = model.normals[f.n[i]];
			Vec2 t = model.texcoords[f.t[i]];

			g.px = v.x; g.py = v.y; g.pz = v.z;
			g.nx = n.x; g.ny = n.y; g.nz = n.z;
			g.u = t.u;  g.v = t.v;

			out.push_back(g);
		}
	}

	return out;
}



GLuint LoadTexture(const char* path) {
	int w, h, c;
	unsigned char* data = stbi_load(path, &w, &h, &c, 4);

	if (!data) {
		printf("Failed to load texture\n");
		return 0;
	}

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
	return tex;
}
bool CreateRenderableObject(
	const char* objPath,
	const char* texPath,
	RenderableObject& outObj
) {
	Model model;

	if (!LoadOBJ(objPath, model))
		return false;

	auto mesh = BuildMesh(model);

	// ---- GPU 버퍼 생성 ----
	GLuint vao, vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER,
		mesh.size() * sizeof(GPUVertex),
		mesh.data(),
		GL_STATIC_DRAW);

	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUVertex), (void*)0);
	glEnableVertexAttribArray(0);

	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GPUVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// uv
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GPUVertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ---- 텍스처 로드 ----
	GLuint tex = LoadTexture(texPath);

	// ---- RenderableObject에 저장 ----
	outObj.VAO = vao;
	outObj.VBO = vbo;
	outObj.textureID = tex;
	outObj.vertexCount = mesh.size();

	outObj.modelMatrix = glm::mat4(1.0f);

	return true;
}
void drawObject(GLuint shader, RenderableObject& obj)
{
	glUseProgram(shader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, obj.textureID);
	glUniform1i(glGetUniformLocation(shader, "tex"), 0);

	glBindVertexArray(obj.VAO);

	// MVP 설정


	glm::mat4 MVP =   obj.modelMatrix;

	glUniformMatrix4fv(
		glGetUniformLocation(shader, "modelTransform"),
		1, GL_FALSE,
		glm::value_ptr(MVP)
	);

	glDrawArrays(GL_TRIANGLES, 0, obj.vertexCount);
}

void deleteObject(RenderableObject& obj) {
	if (obj.VAO) glDeleteVertexArrays(1, &obj.VAO);
	if (obj.VBO) glDeleteBuffers(1, &obj.VBO);
	if (obj.textureID) glDeleteTextures(1, &obj.textureID);
	obj.VAO = obj.VBO = obj.textureID = 0;
}