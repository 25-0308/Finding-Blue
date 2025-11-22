#pragma once
class Lighting {
public:
    glm::vec3 lightPos;
    glm::vec3 lightColor = glm::vec3(1.0f);
    float intensity = 1.0f;

    void apply(GLuint shader) {
        glUseProgram(shader);

        glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform1f(glGetUniformLocation(shader, "lightIntensity"), intensity);
    }
};
