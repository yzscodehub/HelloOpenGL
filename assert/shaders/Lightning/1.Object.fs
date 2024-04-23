#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;

// 材质
struct Material{
	vec3 ambient;
	sampler2D diffuseMap;
	sampler2D specularMap;
	float shininess;		// 反光度
};
uniform Material material;

// 光源
struct Light{
	// vec3 position;
	vec3 direction; // 平行光

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;


void main()
{
	// 环境光
	//float ambientStrengh = 0.1;
	vec3 ambient = light.ambient * vec3(texture(material.diffuseMap, TexCoords));

	// 漫反射
	vec3 norm = normalize(Normal);
	// vec3 lightDir = normalize(lightPos-FragPos);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse =  light.diffuse * diff * vec3(texture(material.diffuseMap, TexCoords));

	// 镜面光
	//float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specularMap, TexCoords));

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}