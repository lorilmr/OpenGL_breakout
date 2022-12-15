#version 330 core//使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。
out vec4 FragColor;//layout (location = 0)设定了输入变量的位置值(Location)

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	FragColor =vec4(1.0); // 将向量的四个分量全部设置为1.0
}