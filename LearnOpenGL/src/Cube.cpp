#include "Cube.h"
#include "Shader.h"

void Cube::setUniforms(const Shader& shader) const
{
	Renderable::setUniforms(shader);
	shader.SetUniform3f("uMaterial.Ambient", GetAmbient());
	shader.SetUniform3f("uMaterial.Diffuse", GetDiffuse());
	shader.SetUniform3f("uMaterial.Specular", GetSpecular());
	shader.SetUniform1f("uMaterial.Shininess", GetShininess());
}

void LightCube::setUniforms(const Shader& shader) const
{
	Renderable::setUniforms(shader);
}
