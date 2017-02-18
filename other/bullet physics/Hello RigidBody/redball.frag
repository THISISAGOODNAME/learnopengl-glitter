#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

out vec4 color;

uniform sampler2D ourTexture;

void main() {
    color = vec4(0.8f, 0.0f, 0.0f, 1.0f);
}