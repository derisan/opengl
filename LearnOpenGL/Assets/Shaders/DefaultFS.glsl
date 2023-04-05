#version 460 core

// in
in vec2 texCoord;
in vec3 normal;
in vec3 fragPosition;

// out
out vec4 fragColor;

// uniform
uniform vec3  uObjectColor;
uniform vec3  uLightColor;
uniform vec3  uLightPosition;
uniform vec3  uViewerPosition;
uniform float uAmbientStrength;
uniform float uSpecularStrength;

void main()
{
	// Ambient
	vec3 ambient = uLightColor * uAmbientStrength;

	// Diffuse
	vec3 lightDirection = normalize(uLightPosition - fragPosition);
	vec3 fragNormal = normalize(normal);
	float diffuseStrength = max(dot(lightDirection, fragNormal), 0.0f);
	vec3 diffuse = uLightColor * diffuseStrength;

	// Specular
	vec3 viewDirection = normalize(uViewerPosition - fragPosition);
	vec3 reflectDirection = reflect(-lightDirection, fragNormal);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32);
	vec3 specular = uLightColor * uSpecularStrength * spec;

	vec3 result = (ambient + diffuse + specular) * uObjectColor;
	fragColor = vec4(result, 1.0f);
}