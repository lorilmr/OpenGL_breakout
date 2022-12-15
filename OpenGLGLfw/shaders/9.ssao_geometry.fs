#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    // 储存片段的位置矢量到第一个G缓冲纹理
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    // 储存法线信息到G缓冲
    gNormal = normalize(Normal);
    // and the diffuse per-fragment color
    // 和漫反射颜色
    gAlbedo.rgb = vec3(0.95);
}