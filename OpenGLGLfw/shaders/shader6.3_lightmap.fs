#version 330 core//使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。
out vec4 FragColor;//layout (location = 0)设定了输入变量的位置值(Location)

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
}; 
struct Light {
    vec3  position;//世界空间位置
    vec3  direction;//方向
    float cutOff;//切光角
    float outerCutOff;//外切角

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Light light;
uniform Material material;
uniform vec3 viewPos;//相机位置（眼睛位置）

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
void main()
{
	vec3 lightDir = normalize(light.position - FragPos);//方向向量归一化
	float theta = dot(lightDir, normalize(-light.direction));

	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));//将环境光得材质颜色设置为漫反射材质颜色同样的值
	
	//漫反射
	vec3 norm = normalize(Normal);//法线归一化
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

	//平滑/软化边缘,将不对环境光做出影响，让它总是能有一点光
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  

	diffuse  *= intensity;
	specular *= intensity;

	// attenuation衰减
	float distance    = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	//消除周围环境的衰减，
	//否则在很远的距离内光线会比聚光灯外暗一些。
	// ambient  *= attenuation; 
	diffuse   *= attenuation;
                specular *= attenuation; 

	vec3 result =specular +ambient + diffuse+emission;

	// 执行光照计算
	FragColor = vec4(result, 1.0);
}