#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

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
	std::vector<std::unique_ptr<Renderable>> objects;
	objects.reserve( 10 );

	for ( auto i = 0; i < 10; ++i )
	{
		auto object = std::make_unique<Renderable>( );
		object->SetTexture( Texture::GetTexture( "Assets/images/container.jpg" ) );
		object->SetTexture( Texture::GetTexture( "Assets/images/awesomeface.png" ) );
		object->SetVertexArray( VertexArray::GetVertexArray( "Cube" ) );
		objects.push_back( std::move( object ) );
	}

	glm::vec3 cubePositions[ ] = {
		glm::vec3( 0.0f,  0.0f,  0.0f ),
		glm::vec3( 2.0f,  5.0f, -15.0f ),
		glm::vec3( -1.5f, -2.2f, -2.5f ),
		glm::vec3( -3.8f, -2.0f, -12.3f ),
		glm::vec3( 2.4f, -0.4f, -3.5f ),
		glm::vec3( -1.7f,  3.0f, -7.5f ),
		glm::vec3( 1.3f, -2.0f, -2.5f ),
		glm::vec3( 1.5f,  2.0f, -2.5f ),
		glm::vec3( 1.5f,  0.2f, -1.5f ),
		glm::vec3( -1.3f,  1.0f, -1.5f )
	};

	Shader shader{ "Assets/Shaders/vs.glsl", "Assets/Shaders/fs.glsl" };

	GLCall( glEnable( GL_DEPTH_TEST ) );

	while ( NOT glfwWindowShouldClose( mWindow.get( ) ) )
	{
		GLCall( glClearColor( 0.2f, 0.3f, 0.3f, 1.0f ) );
		GLCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

		shader.Bind( );

		for ( auto i = 0; i < objects.size( ); i++ )
		{
			auto& object = objects[ i ];

			glm::mat4 model = glm::mat4{ 1.0f };
			model = glm::translate( model, cubePositions[ i ] );
			model = glm::rotate( model, glm::radians( 20.0f * i ), glm::vec3( 1.0f, 0.3f, 0.5f ) );
			glm::mat4 view = glm::mat4{ 1.0f };
			view = glm::translate( view, glm::vec3( 0.0f, 0.0f, -10.0f ) );
			glm::mat4 proj = glm::mat4{ 1.0f };
			proj = glm::perspective( glm::radians( 45.0f ), mScreenWidth / static_cast< float >( mScreenHeight ),
									 0.1f, 100.0f );

			object->SetMVPMatrix( model, view, proj );
			object->Draw( shader );
		}

		glfwSwapBuffers( mWindow.get( ) );
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

