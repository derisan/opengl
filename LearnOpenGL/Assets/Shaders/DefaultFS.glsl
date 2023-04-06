#version 460 core

#define NUM_POINT_LIGHTS 4

// struct
struct Material
{
	sampler2D DiffuseMap;
	sampler2D SpecularMap;
	sampler2D EmissiveMap;
	float Shininess;
};

struct DirLight
{
	vec3 Direction;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

struct PointLight
{
	vec3 Position;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Linear;
	float Quadratic;
};

struct SpotLight
{
	vec3 Position;
	vec3 Direction;
	float Cutoff;
	float OuterCutoff;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Linear;
	float Quadratic;
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
uniform DirLight uDirLight;
uniform PointLight uPointLights[NUM_POINT_LIGHTS];
uniform SpotLight uSpotLight;

// Functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(normal);	
	vec3 viewDir = normalize(uViewerPosition - fragPosition);

	vec3 result = CalcDirLight(uDirLight, norm, viewDir);

	for(int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(uPointLights[i], norm, fragPosition, viewDir);
	}

	result += CalcSpotLight(uSpotLight, norm, fragPosition, viewDir);
	fragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.Direction);
	
	float diff = max(dot(lightDir, normal), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), uMaterial.Shininess);

	vec3 ambient = light.Ambient * vec3(texture(uMaterial.DiffuseMap, texCoord));
	vec3 diffuse = light.Diffuse* diff * vec3(texture(uMaterial.DiffuseMap, texCoord));
	vec3 specular = light.Specular * spec * vec3(texture(uMaterial.SpecularMap, texCoord));

	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.Position - fragPos);

	float diff = max(dot(lightDir, normal), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), uMaterial.Shininess);

	// Attenuation
	float dist = length(light.Position - fragPos);
	float attenuation = 1.0f / (1.0f + light.Linear * dist + 
		light.Quadratic * dist * dist);

	vec3 ambient = light.Ambient * vec3(texture(uMaterial.DiffuseMap, texCoord));
	vec3 diffuse = light.Diffuse * diff * vec3(texture(uMaterial.DiffuseMap, texCoord));
	vec3 specular = light.Specular * spec * vec3(texture(uMaterial.SpecularMap, texCoord));

	return attenuation * (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.Position - fragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), uMaterial.Shininess);

	// Attenuation
	float dist = length(light.Position - fragPos);
	float attenuation = 1.0f / (1.0f + light.Linear * dist + 
		light.Quadratic * dist * dist);

	float theta = dot(lightDir, normalize(-light.Direction));
	float epsilon = light.Cutoff - light.OuterCutoff;
	float intensity = clamp((theta - light.OuterCutoff) / epsilon, 0.0f, 1.0f);

	vec3 ambient = light.Ambient * vec3(texture(uMaterial.DiffuseMap, texCoord));
	vec3 diffuse = light.Diffuse * diff * vec3(texture(uMaterial.DiffuseMap, texCoord));
	vec3 specular = light.Specular * spec * vec3(texture(uMaterial.SpecularMap, texCoord));

	return attenuation * intensity * (ambient + diffuse + specular);
}
