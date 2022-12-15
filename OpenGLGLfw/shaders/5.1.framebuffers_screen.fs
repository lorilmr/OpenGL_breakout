#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
     //无后期处理
    //vec3 col = texture(screenTexture, TexCoords).rgb;
    //FragColor = vec4(col, 1.0);

    //颜色反相
    //FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

    //灰度
    //FragColor = texture(screenTexture, TexCoords);
    //float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    //FragColor = vec4(average, average, average, 1.0);
    
    //核效果
    const float offset = 1.0 / 300.0; 
    vec2 offsets[9] = vec2[](//为周围的纹理坐标创建了一个9个vec2偏移量的数组
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    float kernel[9] = float[](//定义一个锐化核（卷积矩阵）
        //-1, -1, -1,
        //-1,  9, -1,
        //-1, -1, -1
    1.0 / 16, 2.0 / 16, 1.0 / 16,//模糊
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
    );

    vec3 sampleTex[9];//采样周围的所有像素，锐化每个颜色值
    for(int i = 0; i < 9; i++)
    {//采样时我们将每个偏移量加到当前纹理坐标上，获取需要采样的纹理
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];//将这些纹理值乘以加权的核值，并将它们加到一起。

    FragColor = vec4(col, 1.0);
} 