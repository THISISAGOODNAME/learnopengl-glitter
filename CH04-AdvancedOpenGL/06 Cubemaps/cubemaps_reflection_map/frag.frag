#version 330 core
in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 cameraPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_reflection1;
uniform samplerCube skybox;

out vec4 color;

void main()
{             
    // Diffuse
    vec4 diffuse_color = texture(texture_diffuse1, TexCoords);
    // Reflection
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    float reflect_intensity = texture(texture_reflection1, TexCoords).r;
    vec4 reflect_color;
    if(reflect_intensity > 0.1) // Only sample reflections when above a certain treshold
        reflect_color = texture(skybox, R) * reflect_intensity;
    // Combine them
    color = diffuse_color + reflect_color;
}