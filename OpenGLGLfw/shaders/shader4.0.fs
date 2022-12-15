#version 330 core//使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。
out vec4 FragColor;//layout (location = 0)设定了输入变量的位置值(Location)

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;//相机位置（眼睛位置）

in vec3 Normal;
in vec3 FragPos;
void main()
{
	//环境光
	float ambientStrength = 0.1;//常量环境因子
	vec3 ambient = ambientStrength * lightColor;
	//漫反射
	vec3 norm = normalize(Normal);//法线归一化
	vec3 lightDir = normalize(lightPos - FragPos);//方向向量归一化
	float diff = max(dot(norm, lightDir), 0.0);//保证漫反射分量不会变成负数
	vec3 diffuse = diff * lightColor;//漫反射分量
	//镜面反射（高光）
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);//视线方向向量
	vec3 reflectDir = reflect(-lightDir, norm);//对应的沿着法线轴的反射向量(reflect函数要求第一个向量是从光源指向片段位置的向量)
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;//镜面分量

	vec3 result =(specular +ambient + diffuse) * objectColor;
	FragColor = vec4(result, 1.0);
}