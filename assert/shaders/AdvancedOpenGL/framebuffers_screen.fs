#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D DiffuseMap;

// 反相
vec4 Inversion(){
	vec4 color = vec4(vec3(1.0 - texture(DiffuseMap, TexCoords)), 1.0);
	return color;
}

// 灰度
vec4 Grayscale(){
	vec4 color = texture(DiffuseMap, TexCoords);
    // float average = (color.r + color.g + color.b) / 3.0;
	float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    color = vec4(average, average, average, 1.0);
	return color;
}

// 核效果
const float offset = 1.0 / 300.0;  
vec4 KernelEffects()
{
    vec2 offsets[9] = vec2[](
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

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

	// 模糊
	float blur_kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16  
	);
	
	// 边缘检测
	float edge_detection_kernel[9] = float[](
        1,  1, 1,
        1, -8, 1,
        1,  1, 1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(DiffuseMap, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * edge_detection_kernel[i];

    vec4 color = vec4(col, 1.0);
	return color;
}


void main()
{
	// FragColor = texture(DiffuseMap, TexCoords);
	// 反相
	// FragColor = Inversion();
	// 灰度
	// FragColor = Grayscale();
	// 核效果
	FragColor = KernelEffects();
}