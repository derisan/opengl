#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <iostream>

#include "Macros.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
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

	{
		// Test
		float vertices[ ] = {
		-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
		 -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		unsigned int indices[ ] = {
			0, 1, 2,
			0, 2, 3
		};

		VertexArray va;
		VertexBuffer vb{ vertices, sizeof( vertices ) };
		VertexLayout layout;
		layout.Push<float>( 3 );
		layout.Push<float>( 2 );
		va.SetVertexBuffer( vb, layout );

		IndexBuffer ib{ indices, static_cast< unsigned int >( std::size( indices ) ) };
		va.SetIndexBuffer( ib );

		Shader shader{ "Assets/Shaders/vs.glsl", "Assets/Shaders/fs.glsl" };

		Texture texture1{ "Assets/images/container.jpg" };
		Texture texture2{ "Assets/images/awesomeface.png" };
		texture1.Bind( 0 );
		texture2.Bind( 1 );
		shader.SetUniform1i( "texture1", 0 );
		shader.SetUniform1i( "texture2", 1 );

		while ( NOT glfwWindowShouldClose( window ) )
		{
			GLCall( glClearColor( 0.2f, 0.3f, 0.3f, 1.0f ) );
			GLCall( glClear( GL_COLOR_BUFFER_BIT ) );

			shader.Bind( );
			va.Bind( );

			glm::mat4 mvp = glm::mat4{ 1.0f };
			mvp = glm::translate( mvp, glm::vec3( 0.5f, 0.5f, 0.0f ) );
			mvp = glm::rotate( mvp, glm::radians( static_cast< float >( glfwGetTime( ) ) * 30.0f ),
							   glm::vec3( 0.0f, 0.0f, 1.0f ) );
			mvp = glm::scale( mvp, glm::vec3( 0.5f, 0.5f, 0.5f ) );
			shader.SetUniformMat4( "uMVP", mvp );

			GLCall( glDrawElements( GL_TRIANGLES, va.GetNumIndices( ), GL_UNSIGNED_INT, nullptr ) );

			glfwSwapBuffers( window );

			glfwPollEvents( );
		}
	}

	glfwTerminate( );
	return 0;
}