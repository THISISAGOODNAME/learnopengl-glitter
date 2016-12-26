#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture1;

float LinearizeDepth(float depth)
{
    float near = 0.1;
    float far = 100.0;
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near) / (far + near - z * (far - near));
}

void main()
{             
    float depth = LinearizeDepth(gl_FragCoord.z);
    //color = texture(texture1, TexCoords);
    color = vec4(vec3(depth), 1.0f);
}