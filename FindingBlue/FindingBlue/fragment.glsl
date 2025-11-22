#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex;        //  텍스처
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float lightIntensity;
uniform bool useLight;

void main()
{
    // 텍스처 색 읽기
    vec3 baseColor = texture(tex, TexCoord).rgb;

    // ───── Ambient ─────
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor * baseColor;

    // ───── Diffuse ─────
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * baseColor;

    // ───── Specular ─────
    float shininess = 128.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * lightColor;

    vec3 result;
    if (useLight)
        result = (ambient + diffuse + specular) * lightIntensity;
    else
        result = baseColor;

    FragColor = vec4(result, 1.0);
}
