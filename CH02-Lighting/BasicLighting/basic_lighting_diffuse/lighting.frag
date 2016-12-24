#version 330 core

out vec4 color;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
	// 环境光Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

	// 漫反射Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
	color = vec4(result, 1.0f);
}
