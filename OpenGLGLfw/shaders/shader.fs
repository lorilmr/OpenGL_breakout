#version 330 core//使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。
out vec4 FragColor;//layout (location = 0)设定了输入变量的位置值(Location)
//in vec4 vertexColor;// 从顶点着色器传来的输入变量（名称相同、类型相同）
//uniform vec4 ourColor; // uniform 在OpenGL程序代码中设定这个变量
in vec3 ourColor;
in vec2 TexCoord;
//uniform sampler2D ourTexture;

uniform float mixValue;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), mixValue);//仅让笑脸图案朝着另一个方向看
	//FragColor = texture(ourTexture, TexCoord)*vec4(ourColor,1.0f);
	//FragColor =ourColor;//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);//gl_Position设置的值会成为该顶点着色器的输出。
}