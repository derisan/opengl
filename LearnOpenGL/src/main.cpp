#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <iostream>
#include <vector>

#include "Macros.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Renderable.h"

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
		auto object = std::make_unique<Renderable>( );
		object->SetTexture( Texture::GetTexture( "Assets/images/container.jpg" ) );
		object->SetTexture( Texture::GetTexture( "Assets/images/awesomeface.png" ) );
		object->SetVertexArray( VertexArray::GetVertexArray( "Rectangle" ) );

		auto object2 = std::make_unique<Renderable>( );
		object2->SetTexture( Texture::GetTexture( "Assets/images/container.jpg" ) );
		object2->SetTexture( Texture::GetTexture( "Assets/images/carrot.png" ) );
		object2->SetVertexArray( VertexArray::GetVertexArray( "Rectangle" ) );

		Shader shader{ "Assets/Shaders/vs.glsl", "Assets/Shaders/fs.glsl" };

		while ( NOT glfwWindowShouldClose( window ) )
		{
			GLCall( glClearColor( 0.2f, 0.3f, 0.3f, 1.0f ) );
			GLCall( glClear( GL_COLOR_BUFFER_BIT ) );

			shader.Bind( );

			glm::mat4 wvp = glm::mat4{ 1.0f };
			wvp = glm::translate( wvp, glm::vec3( 0.5f, 0.5f, 0.0f ) );
			wvp = glm::rotate( wvp, glm::radians( static_cast< float >( glfwGetTime( ) ) * 30.0f ),
							   glm::vec3( 0.0f, 0.0f, 1.0f ) );
			wvp = glm::scale( wvp, glm::vec3( 0.5f, 0.5f, 0.5f ) );

			object->SetWVPMatrix( wvp );
			object->Draw( shader );

			wvp = glm::mat4{ 1.0f };
			wvp = glm::translate( wvp, glm::vec3( -0.5f, -0.5f, 0.0f ) );
			wvp = glm::rotate( wvp, glm::radians( static_cast< float >( glfwGetTime( ) ) * 30.0f ),
							   glm::vec3( 0.0f, 0.0f, 1.0f ) );
			wvp = glm::scale( wvp, glm::vec3( 0.5f, 0.5f, 0.5f ) );

			object2->SetWVPMatrix( wvp );
			object2->Draw( shader );

			glfwSwapBuffers( window );

			glfwPollEvents( );
		}

		Texture::Clear( );
		VertexArray::Clear( );
	}

	glfwTerminate( );
	return 0;
}