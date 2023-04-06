#version 460 core

// struct
struct Material
{
	sampler2D DiffuseMap;
	sampler2D SpecularMap;
	sampler2D EmissiveMap;
	float Shininess;
};

struct Light
{
	vec3 Position;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

// in
in vec2 texCoord;
in vec3 normal;
in vec3 fragPosition;

// out
out vec4 fragColor;

// uniform
uniform vec3	 uViewerPosition;
uniform Material uMaterial;
uniform Light    uLight;

void main()
{
	// Emmisive
	vec3 emmisive = vec3(texture(uMaterial.EmissiveMap, texCoord));

	// Ambient
	vec3 ambient = uLight.Ambient * vec3(texture(uMaterial.DiffuseMap, texCoord));

	// Diffuse
	vec3 lightDirection = normalize(uLight.Position - fragPosition);
	vec3 fragNormal = normalize(normal);
	float diffuseStrength = max(dot(lightDirection, fragNormal), 0.0f);
	vec3 diffuse = uLight.Diffuse * diffuseStrength 
					* vec3(texture(uMaterial.DiffuseMap, texCoord));

	// Specular
	vec3 viewDirection = normalize(uViewerPosition - fragPosition);
	vec3 reflectDirection = reflect(-lightDirection, fragNormal);
	float specularStrength = pow(max(dot(viewDirection, reflectDirection), 0.0f), 
								uMaterial.Shininess);
	vec3 specular = uLight.Specular * specularStrength 
					* vec3(texture(uMaterial.SpecularMap, texCoord));

	vec3 result = emmisive + ambient + diffuse + specular;
	fragColor = vec4(result, 1.0f);
}