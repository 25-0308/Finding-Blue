#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 modelTransform;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * modelTransform * vec4(vPos, 1.0);

    FragPos = vec3(modelTransform * vec4(vPos, 1.0));
    Normal = mat3(transpose(inverse(modelTransform))) * vNormal;

    TexCoord = vTexCoord;  // ★ 텍스쳐 전달
}
