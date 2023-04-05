#pragma once

#include "Renderable.h"


class Shader;


class Cube :
	public Renderable
{
private:
	virtual void setUniforms(const Shader& shader) const override;
};


class LightCube :
	public Renderable
{
private:
	virtual void setUniforms(const Shader& shader) const override;
};

