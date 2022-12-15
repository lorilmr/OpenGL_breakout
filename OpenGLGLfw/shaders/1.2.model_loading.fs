#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 Position;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_height1;//texture_reflect1
uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{    
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));//反射向量
    //vec3 R = refract(I, normalize(Normal), ratio);//折射向量
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);
    //FragColor = texture(texture_diffuse1, TexCoords);
    //FragColor = texture(texture_height1, TexCoords);
    FragColor = vec4(texture(skybox, R).rgb, 1.0)*texture(texture_height1, TexCoords)+texture(texture_diffuse1, TexCoords);
}