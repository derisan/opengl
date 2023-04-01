#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <iostream>

#include "Macros.h"

const unsigned int kScreenWidth = 800;
const unsigned int kScreenHeight = 600;

void OnWindowSizeChange( GLFWwindow* window, int w, int h )
{
	glViewport( 0, 0, w, h );
}

void OnKeydown( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
	{
		glfwSetWindowShouldClose( window, true );
	}
}

int main( )
{
	GLFWwindow* window;

	if ( NOT glfwInit( ) )
	{
		return -1;
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	window = glfwCreateWindow( kScreenWidth, kScreenHeight, "Learn OpenGL", NULL, NULL );
	if ( NOT window )
	{
		glfwTerminate( );
		return -1;
	}

	glfwMakeContextCurrent( window );

	glfwSetFramebufferSizeCallback( window, OnWindowSizeChange );
	glfwSetKeyCallback( window, OnKeydown );

	auto err = glewInit( );
	if ( GLEW_OK != err )
	{
		std::cout << glewGetErrorString( err ) << std::endl;
		return -1;
	}

	while ( NOT glfwWindowShouldClose( window ) )
	{
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		glfwSwapBuffers( window );

		glfwPollEvents( );
	}

	glfwTerminate( );
	return 0;
}