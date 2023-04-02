#pragma once

#include <glew/glew.h>

#include <string_view>
#include <iostream>

#define NOT !
#define ASSERT(x) if(!(x)) { __debugbreak(); }

void GLClearError( );

bool GLLogError( std::string_view functionName, std::string_view fileName, int lineNumber );

#ifdef _DEBUG
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogError(#x, __FILE__, __LINE__));
#else
#define GLCall(x) x;
#endif