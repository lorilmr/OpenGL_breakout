#version 330 core//使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。
layout (location = 0) in vec3 aPos;//layout (location = 0)设定了输入变量的位置值(Location)
layout (location = 1) in vec3 aColor;// 颜色变量的属性位置值为 1
layout (location = 2) in vec2 aTexCoord;//纹理变量的属性位置值2
//uniform vec4 movePos;
out vec3 ourColor;// 向片段着色器输出一个颜色
out vec2 TexCoord;//向片段着色器输出纹理坐标
//out vec4 vertexColor;// 为片段着色器指定一个颜色输出

uniform mat4 transform;//变换矩阵

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//gl_Position = transform*vec4(aPos,1.0);//gl_Position设置的值会成为该顶点着色器的输出。
	gl_Position = projection*view*model*vec4(aPos,1.0);
	ourColor = aColor; //将ourColor设置为我们从顶点数据那里得到的输入颜色
	TexCoord = aTexCoord;//赋值纹理坐标，并传给fs
	//TexCoord=vec2(aTexCoord.x,1-aTexCoord.y);
	//vertexColor = vec4(0.5, 0.0, 0.0, 1.0);// 把输出变量设置为暗红色
}