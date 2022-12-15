#version 330 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

// tile noise texture over screen based on screen dimensions divided by noise size
// 屏幕的平铺噪声纹理会根据屏幕分辨率除以噪声大小的值来决定
// 我们将通过屏幕分辨率除以噪声纹理大小的方式计算TexCoords的缩放大小，并在之后提取相关输入向量的时候使用。
const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0);  // 屏幕 = 800x600

uniform mat4 projection;

void main()
{
    // get input for SSAO algorithm
    // 获取SSAO算法的输入
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    // create TBN change-of-basis matrix: from tangent-space to view-space
    // 由于我们将texNoise的平铺参数设置为GL_REPEAT，随机的值将会在全屏不断重复。
    // 加上fragPog和normal向量，我们就有足够的数据来创建一个TBN矩阵，将向量从切线空间变换到观察空间。
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        // / 获取样本位置
        vec3 samplePos = TBN * samples[i]; // from tangent to view-space切线->观察空间
        samplePos = fragPos + samplePos * radius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset; // from view to clip-space// 观察->裁剪空间
        offset.xyz /= offset.w; // perspective divide// 透视划分
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0// 变换到0.0 - 1.0的值域
        
        // get sample depth
        float sampleDepth = texture(gPosition, offset.xy).z; // get depth value of kernel sample获取核样本的深度值
        
        // range check & accumulate
        // 我们引入一个范围测试从而保证我们只当被测深度值在取样半径内时影响遮蔽因子。
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        // 检查样本的当前深度值是否大于存储的深度值，如果是的，添加到最终的贡献因子上
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
    //最后一步，我们需要将遮蔽贡献根据核心的大小标准化，并输出结果。注意我们用1.0减去了遮蔽因子，以便直接使用遮蔽因子去缩放环境光照分量。
    occlusion = 1.0 - (occlusion / kernelSize);
    
    FragColor = occlusion;
}