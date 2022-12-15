#version 330 core//使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。
out vec4 FragColor;//layout (location = 0)设定了输入变量的位置值(Location)

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
}; 
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;//Kc
    float linear;//Kl
    float quadratic;//Kq
};
uniform Light light;
uniform Material material;
uniform vec3 viewPos;//相机位置（眼睛位置）

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
void main()
{
	float distance    = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));//将环境光得材质颜色设置为漫反射材质颜色同样的值

	//漫反射
	vec3 norm = normalize(Normal);//法线归一化
	vec3 lightDir = normalize(light.position - FragPos);//方向向量归一化
	float diff = max(dot(norm, lightDir), 0.0);//保证漫反射分量不会变成负数
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	//镜面反射（高光）
	vec3 viewDir = normalize(viewPos - FragPos);//视线方向向量
	vec3 reflectDir = reflect(-lightDir, norm);//对应的沿着法线轴的反射向量(reflect函数要求第一个向量是从光源指向片段位置的向量)
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//vec3 specular = (spec * material.specular)* light.specular;//镜面分量
	vec3 specular=light.specular * spec * vec3(texture(material.specular, TexCoords));
	
	//放射光
	vec3 emission = texture(material.emission, TexCoords).rgb;
	
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 result =specular +ambient + diffuse+emission;
	FragColor = vec4(result, 1.0);
}