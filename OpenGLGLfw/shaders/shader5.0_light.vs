#version 330 core//使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。
layout (location = 0) in vec3 aPos;//layout (location = 0)设定了输入变量的位置值(Location)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection*view*model*vec4(aPos,1.0);
}