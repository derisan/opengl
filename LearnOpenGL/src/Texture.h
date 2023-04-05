#pragma once

#include <string_view>
#include <unordered_map>
#include <memory>
#include "GLObject.h"


class Texture :
	public GLObject
{
	using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;

public:
	static std::shared_ptr<Texture> GetTexture(std::string_view fileName);
	static void Clear();

private:
	inline static TextureMap sTextures;

public:
	Texture() = default;
	Texture(std::string_view fileName);
	~Texture();

	void LoadTexture(std::string_view fileName);
	void Bind(unsigned int slot) const;
	virtual void Unbind() const override;

	int GetWidth() const;
	int GetHeight() const;

private:
	virtual void Bind() const override;

private:
	int mWidth = 0;
	int mHeight = 0;
};

static std::string GetFileExtension(std::string_view fileName);