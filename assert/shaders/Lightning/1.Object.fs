#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	// 环境光照
	float ambientStrengh = 0.1;
	vec3 ambient = ambientStrengh*lightColor;

	// 漫反射光照
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos-FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// 镜面光照
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}