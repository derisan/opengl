#pragma once

#include "GLObject.h"

#include <string_view>
#include <unordered_map>
#include <memory>

class Texture:
    public GLObject
{
public:
    static std::shared_ptr<Texture> GetTexture( std::string_view fileName );
    static void Clear( );

	Texture( std::string_view fileName );
	~Texture( );

	void Bind( unsigned int slot ) const;
	virtual void Unbind( ) const override;
	int GetWidth( ) const;
	int GetHeight( ) const;

private:
    virtual void Bind( ) const override;

    void loadTexture( std::string_view fileName );

private:
    inline static std::unordered_map<std::string, std::shared_ptr<Texture>> sTextures;

    int mWidth = { 0 };
    int mHeight = { 0 };
};

