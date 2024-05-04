#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D DiffuseMap;

void main()
{
	vec4 texColor = texture(DiffuseMap, TexCoords);
    FragColor = texColor;
}