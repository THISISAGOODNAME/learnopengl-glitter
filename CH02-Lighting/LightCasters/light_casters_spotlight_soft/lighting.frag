#version 330 core

out vec4 color;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
uniform Light light;

in vec3 Normal;
in vec3 FragPos;

in vec2 TexCoords;

uniform vec3 viewPos;

void main() {
	// 环境光Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// 漫反射Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// 镜面反射Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	// 点光源Spotlight (soft edges)
	float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

	// 光强衰减Attenuation
	float distance    = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;     
    
	vec3 result = ambient + diffuse + specular; // Also remove attenuation from ambient, because if we move too far, the light in spotlight would then be darker than outside (since outside spotlight we have ambient lighting)
	color = vec4(result, 1.0f);
}
