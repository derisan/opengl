#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <iostream>

#include "Macros.h"

int main( void )
{
	GLFWwindow* window;

	if ( NOT glfwInit( ) )
	{
		return -1;
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	window = glfwCreateWindow( 640, 480, "Learn OpenGL", NULL, NULL );
	if ( NOT window )
	{
		glfwTerminate( );
		return -1;
	}

	glfwMakeContextCurrent( window );

	auto err = glewInit( );
	if ( GLEW_OK != err )
	{
		std::cout << glewGetErrorString( err ) << std::endl;
		return -1;
	}
	
	std::cout << glGetString( GL_VERSION ) << std::endl;

	while ( NOT glfwWindowShouldClose( window ) )
	{
		glClear( GL_COLOR_BUFFER_BIT );

		glfwSwapBuffers( window );

		glfwPollEvents( );
	}

	glfwTerminate( );
	return 0;
}