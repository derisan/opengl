#pragma once

#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <string_view>
#include <memory>
#include <functional>

struct glfwDeleter
{
	void operator()( GLFWwindow* window )
	{
	}
};

class Application
{
public:
	Application( std::string_view title, int width, int height, int glVersionMajor,
				 int glVersionMinor);
	~Application( );

	void Run( );

	void SetViewport( int width, int height );
	int GetScreenWidth( ) const;
	int GetScreenHeight( ) const;

private:
	void createWindowAndContext( std::string_view title, int width, int height, 
					   int glVersionMajor, int glVersionMinor );
	void initGLEW( );
	void setCallbacks( );

private:
	std::unique_ptr<GLFWwindow, glfwDeleter> mWindow;
	int mScreenWidth = { 0 };
	int mScreenHeight = { 0 };
};

