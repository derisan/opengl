#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "Macros.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Renderable.h"



Application::Application( std::string_view title, int width, int height, int glVersionMajor, int glVersionMinor )
	: mScreenWidth{ width }
	, mScreenHeight{ height }
{
	createWindowAndContext( title, width, height, glVersionMajor, glVersionMinor );
	setCallbacks( );
	initGLEW( );
}

Application::~Application( )
{
	Texture::Clear( );
	VertexArray::Clear( );
	glfwTerminate( );
}

void Application::Run( )
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

	while ( NOT glfwWindowShouldClose( mWindow.get( ) ) )
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

		glfwSwapBuffers( mWindow.get() );
		glfwPollEvents( );
	}
}

void Application::SetViewport( int width, int height )
{
	mScreenWidth = width;
	mScreenHeight = height;
	glViewport( 0, 0, width, height );
}

int Application::GetScreenWidth( ) const
{
	return mScreenWidth;
}

int Application::GetScreenHeight( ) const
{
	return mScreenHeight;
}

void Application::createWindowAndContext( std::string_view title, int width, int height, int glVersionMajor, int glVersionMinor )
{
	if ( NOT glfwInit( ) )
	{
		ASSERT( false );
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, glVersionMinor );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	mWindow = std::unique_ptr<GLFWwindow, glfwDeleter>{ glfwCreateWindow( width, height, title.data( ), nullptr, nullptr ) };

	if ( NOT mWindow )
	{
		ASSERT( false );
	}

	glfwMakeContextCurrent( mWindow.get( ) );
}

void Application::initGLEW( )
{
	if ( auto err = glewInit( ); GLEW_OK != err )
	{
		std::cout << glewGetErrorString( err ) << std::endl;
		ASSERT( false );
	}

	std::cout << glGetString( GL_VERSION ) << std::endl;
}

void Application::setCallbacks( )
{
	glfwSetKeyCallback( mWindow.get( ), [ ]( GLFWwindow* window, int key, int scancode, int action, int mods )
						{
							if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
							{
								glfwSetWindowShouldClose( window, true );
							}
						} );

	glfwSetFramebufferSizeCallback( mWindow.get( ), [ ]( GLFWwindow* window, int w, int h )
									{
										auto app = reinterpret_cast< Application* >( window );
										app->SetViewport( w, h );
									} );
}

