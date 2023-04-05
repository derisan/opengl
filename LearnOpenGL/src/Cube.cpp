#include "Cube.h"
#include "Shader.h"

void Cube::setUniforms(const Shader& shader) const
{
	Renderable::setUniforms(shader);
	shader.SetUniform3f("uObjectColor", GetColor());
}

void LightCube::setUniforms(const Shader& shader) const
{
	Renderable::setUniforms(shader);
}
