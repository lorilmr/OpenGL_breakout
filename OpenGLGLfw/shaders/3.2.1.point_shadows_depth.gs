#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.它指定发散出基本图形送到立方体贴图的哪个面。当然这只有当我们有了一个附加到激活的帧缓冲的立方体贴图纹理才有效
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * FragPos;//将每个世界空间顶点变换到相关的光空间
            EmitVertex();
        }    
        EndPrimitive();
    }
} 