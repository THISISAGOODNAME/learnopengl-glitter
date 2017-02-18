#version 330 core

in VS_OUT {
    vec2 TexCoords;
} fs_in;

out vec4 color;

uniform sampler2D ourTexture;

void main() {
    color = texture(ourTexture, fs_in.TexCoords);
}