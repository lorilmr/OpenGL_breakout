#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;//单颜色通道的字形位图纹理
uniform vec3 textColor;//颜色

void main()
{    
//首先从位图纹理中采样颜色值，由于纹理数据中仅存储着红色分量，
//我们就采样纹理的r分量来作为取样的alpha值。
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}