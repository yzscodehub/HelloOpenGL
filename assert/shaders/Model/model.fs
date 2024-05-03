#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

// 材质
struct Material{
	vec3 ambient;
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;		// 反光度
};
uniform Material material;


// 定向光
struct DirLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);
	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
	return ambient + diffuse + specular;
}

// 点光源
struct PointLight{
	vec3 position;

	// 光源衰减参数
	float constant;		// 常数项
	float linear;		// 一次项
	float quadratic;	// 二次项

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define NR_POINT_LIGHT 4
uniform PointLight pointLights[NR_POINT_LIGHT];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(light.position-fragPos);
	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// 衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
		light.quadratic * (distance * distance));
	
	// 合并结果
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return ambient + diffuse + specular;
}

// 聚光灯
struct SpotLight{
	vec3 position;
	vec3 direction; 	// 平行光
	float cutOff;		// 切光角的余弦值
	float outerCutOff;	// 外切光角的余弦值

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

		// 光源衰减参数
	float constant;		// 常数项
	float linear;		// 一次项
	float quadratic;	// 二次项

};
uniform SpotLight spotLight;
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(light.position-fragPos);
	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

	// 衰减
	float distance = length(light.position-fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
	light.quadratic * (distance* distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// 平滑边缘
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta-light.outerCutOff)/epsilon, 0.0, 1.0);
	diffuse  *= intensity;
	specular *= intensity;

	return ambient + diffuse + specular;
}

void main(){
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// 第一阶段：定向光源
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	// 第二阶段：点光源
	for(int i = 0; i < NR_POINT_LIGHT; ++i){
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	// 第三阶段：聚光
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
	FragColor = vec4(result, 1.0);
}