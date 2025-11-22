#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float lightIntensity;
uniform bool useLight;

struct Material {
    float shininess;
    float specularStrength;
    float metallic; // 원하면 스펙 반영할 때 사용
};

uniform Material material;

void main()
{
    vec3 baseColor = texture(tex, TexCoord).rgb;

    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 specular = spec * material.specularStrength * lightColor;

    vec3 lighting = (ambient + diffuse + specular) * baseColor * lightIntensity;

    FragColor = vec4(useLight ? lighting : baseColor, 1.0);
}
