#version 460 core

in vec2 texCoord;
in vec3 normal;

out vec4 FragColor;

uniform vec3 uObjectColor;
uniform vec3 uLightColor;

void main()
{
	FragColor = vec4(uLightColor * uObjectColor, 1.0f);
}