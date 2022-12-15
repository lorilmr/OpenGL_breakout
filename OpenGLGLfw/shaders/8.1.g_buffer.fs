#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
// 将片段位置矢量存储在第一个gBuffer纹理中
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
// 还将每段法线存储到gBuffer中。
    gNormal = normalize(Normal);
    // and the diffuse per-fragment color
// 在gAlbedoSpec的rgb分量中存储没片段的漫反射颜色
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
// 在gAlbedoSpec的a分量中存储镜面强度
    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}