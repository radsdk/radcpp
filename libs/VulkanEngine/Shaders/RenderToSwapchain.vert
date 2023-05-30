#version 450 core

layout (location = 0) in vec4 in0_PositionTexCoord;
layout (location = 0) out vec2 out0_TexCoord;

void main()
{
    gl_Position = vec4(in0_PositionTexCoord.xy, 0.0f, 1.0f);
    out0_TexCoord = in0_PositionTexCoord.zw;
}
