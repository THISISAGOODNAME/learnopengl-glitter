#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

void build_house(vec4 position)
{
    fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex
    gl_Position = position + vec4(-0.2f, -0.2f, 0.0f, 0.0f);// 1:左下角
    EmitVertex();
    gl_Position = position + vec4( 0.2f, -0.2f, 0.0f, 0.0f);// 2:右下角
    EmitVertex();
    gl_Position = position + vec4(-0.2f,  0.2f, 0.0f, 0.0f);// 3:左上
    EmitVertex();
    gl_Position = position + vec4( 0.2f,  0.2f, 0.0f, 0.0f);// 4:右上
    EmitVertex();
    gl_Position = position + vec4( 0.0f,  0.4f, 0.0f, 0.0f);// 5:屋顶
    fColor = vec3(1.0f, 1.0f, 1.0f); // 将房顶的颜色设为白色，显示下雪的效果
    EmitVertex();
    EndPrimitive();
}

void main()
{
    build_house(gl_in[0].gl_Position);
}