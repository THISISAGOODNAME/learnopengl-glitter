#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
const float offset = 1.0 / 300;
void main()
{
    // 无后期处理
    color = texture(screenTexture, TexCoords);

    // 反相
    // color = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

    // 灰度
    // 平均灰度
    // color = texture(screenTexture, TexCoords);
    // float average = (color.r + color.g + color.b) / 3.0;
    // color = vec4(average, average, average, 1.0);
    // 加权灰度
    // color = texture(screenTexture, TexCoords);
    // float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    // color = vec4(average, average, average, 1.0);

    // // Kernel effects

    // vec2 offsets[9] = vec2[](
    //     vec2(-offset, offset),  // top-left
    //     vec2(0.0f,    offset),  // top-center
    //     vec2(offset,  offset),  // top-right
    //     vec2(-offset, 0.0f),    // center-left
    //     vec2(0.0f,    0.0f),    // center-center
    //     vec2(offset,  0.0f),    // center-right
    //     vec2(-offset, -offset), // bottom-left
    //     vec2(0.0f,    -offset), // bottom-center
    //     vec2(offset,  -offset)  // bottom-right
    // );

    // // 锐化kernel
    // // float kernel[9] = float[](
    // //     -1, -1, -1,
    // //     -1,  9, -1,
    // //     -1, -1, -1
    // // );

    // // 模糊kernel
    // // float kernel[9] = float[](
    // //     1.0 / 16, 2.0 / 16, 1.0 / 16,
    // //     2.0 / 16, 4.0 / 16, 2.0 / 16,
    // //     1.0 / 16, 2.0 / 16, 1.0 / 16  
    // // );

    // // 边缘检测kernel
    // float kernel[9] = float[](
    //     1,  1, 1,
    //     1, -8, 1,
    //     1,  1, 1
    // );

    // vec3 sampleTex[9];
    // for(int i = 0; i < 9; i++)
    // {
    //     sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    // }
    // vec3 col;
    // for(int i = 0; i < 9; i++)
    //     col += sampleTex[i] * kernel[i];

    // color = vec4(col, 1.0);

}