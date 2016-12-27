#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoords;

out VS_OUT {
    vec2 texCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); 
    vs_out.texCoords = texCoords;
}