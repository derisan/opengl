#include "Cube.h"
#include "Shader.h"

void Cube::setUniforms(const Shader& shader) const
{
	Renderable::setUniforms(shader);
	shader.SetUniform1f("uMaterial.Shininess", GetShininess());
}

void LightCube::setUniforms(const Shader& shader) const
{
	Renderable::setUniforms(shader);
}
