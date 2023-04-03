#pragma once

#include "GLObject.h"

#include <string_view>

class Texture:
    public GLObject
{
public:
    Texture( std::string_view fileName );
    ~Texture( );

    void Bind( unsigned int slot ) const;

    virtual void Unbind( ) const override;

    int GetWidth( ) const;
    int GetHeight( ) const;

private:
    virtual void Bind( ) const override;

private:
    void loadTexture( std::string_view fileName );

private:
    int mWidth = { 0 };
    int mHeight = { 0 };
};

