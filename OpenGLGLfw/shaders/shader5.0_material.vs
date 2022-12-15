#version 330 core//使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。
layout (location = 0) in vec3 aPos;//layout (location = 0)设定了输入变量的位置值(Location)
layout (location = 1) in vec3 aNormal;//顶点法向量
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos; //把顶点位置属性乘以模型矩阵（不是观察和投影矩阵）来把它变换到世界空间坐标
void main()
{
	gl_Position = projection*view*model*vec4(aPos,1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal=aNormal;
}