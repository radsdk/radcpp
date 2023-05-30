#version 450 core

layout (set = 0, binding = 0) uniform sampler2D g_colorBuffer;
layout (set = 0, binding = 1) uniform sampler2D g_overlay;

layout (location = 0) in vec2 in0_TexCoord;
layout (location = 0) out vec4 out0_FragColor;

void main()
{
    vec4 color = texture(g_colorBuffer, in0_TexCoord);
    vec4 overlayColor = texture(g_overlay, in0_TexCoord);
    out0_FragColor.rgb = overlayColor.rgb + color.rgb * (1.0f - overlayColor.a);
    out0_FragColor.a = 1.0f;
}
