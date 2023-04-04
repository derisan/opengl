#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <algorithm>

#include "Macros.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Renderable.h"
#include "Camera.h"


void KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods );
void MouseCallback( GLFWwindow* window, double x, double y );
void MouseScrollCallback( GLFWwindow* window, double xOffset, double yOffset );
void WindowSizeCallback( GLFWwindow* window, int w, int h );

Application& Application::Get( )
{
	static Application app;
	return app;
}

Application::~Application( )
{
	Texture::Clear( );
	VertexArray::Clear( );
	glfwTerminate( );
}

void Application::Init( std::string_view title, int width, int height, int glVersionMajor, int glVersionMinor )
{
	mScreenWidth = width;
	mScreenHeight = height;

	createWindowAndContext( title, width, height, glVersionMajor, glVersionMinor );
	setCallbacks( );
	initGLEW( );
	createCamera( );
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

	float lastFrame = static_cast< float >( glfwGetTime( ) );
	float currentFrame = lastFrame;

	while ( NOT glfwWindowShouldClose( mWindow ) )
	{
		currentFrame = static_cast< float >( glfwGetTime( ) );
		mDeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		GLCall( glClearColor( 0.2f, 0.3f, 0.3f, 1.0f ) );
		GLCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

		shader.Bind( );

		mCamera->Bind( shader );

		for ( auto i = 0; i < objects.size( ); i++ )
		{
			auto& object = objects[ i ];

			glm::mat4 model = glm::mat4{ 1.0f };
			model = glm::translate( model, cubePositions[ i ] );
			model = glm::rotate( model, glm::radians( 20.0f * i ), glm::vec3( 1.0f, 0.3f, 0.5f ) );
			object->SetModelMatrix( model );
			object->Draw( shader );
		}

		glfwSwapBuffers( mWindow );
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

void Application::Shutdown( )
{
	glfwSetWindowShouldClose( mWindow, true );
}

void Application::MoveCamera( int keycode )
{
	const float cameraSpeed = 50.0f * mDeltaTime;

	auto cameraPos = mCamera->GetCameraPos( );

	switch ( keycode )
	{
		case GLFW_KEY_W:
			cameraPos += mCamera->GetCameraFront( ) * cameraSpeed;
			break;
		case GLFW_KEY_S:
			cameraPos -= mCamera->GetCameraFront( ) * cameraSpeed;
			break;
		case GLFW_KEY_A:
			cameraPos -= glm::cross( mCamera->GetCameraFront( ), mCamera->GetCameraUp( ) )
				* cameraSpeed;
			break;
		case GLFW_KEY_D:
			cameraPos += glm::cross( mCamera->GetCameraFront( ), mCamera->GetCameraUp( ) )
				* cameraSpeed;
			break;
		default:
			ASSERT( false );
			break;
	}

	mCamera->SetCameraPos( cameraPos );
}

void Application::TurnCamera( const glm::vec3& direction )
{
	mCamera->SetCameraFront( direction );
}

void Application::ZoomCamera( float offset )
{
	auto fov = mCamera->GetFOV( );

	// Scroll Up -> offset = 1
	// Scroll Down -> offset = -1
	fov -= offset;

	fov = std::clamp( fov, 1.0f, 60.0f );
	mCamera->SetFOV( fov );
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

	mWindow = glfwCreateWindow( width, height, title.data( ), nullptr, nullptr );

	if ( NOT mWindow )
	{
		ASSERT( false );
	}

	// 마우스 가두기
	glfwSetInputMode( mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

	glfwMakeContextCurrent( mWindow );
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
	glfwSetKeyCallback( mWindow, KeyCallback );
	glfwSetCursorPosCallback( mWindow, MouseCallback );
	glfwSetFramebufferSizeCallback( mWindow, WindowSizeCallback );
	glfwSetScrollCallback( mWindow, MouseScrollCallback );
}

void Application::createCamera( )
{
	mCamera = std::make_unique<Camera>( );
	mCamera->SetViewMatrix( glm::vec3{ 0.0f, 0.0f, 10.0f },
							glm::vec3{ 0.0f, 0.0f, -1.0f },
							glm::vec3{ 0.0f, 1.0f, 0.0f } );
	mCamera->SetPerspectiveMatrix( 45.0f,
								   mScreenWidth / static_cast< float >( mScreenHeight ),
								   0.1f, 100.0f );
}

void KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	auto& app = Application::Get( );

	if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
	{
		app.Shutdown( );
	}

	if ( ( GLFW_KEY_W == key ||
		   GLFW_KEY_S == key ||
		   GLFW_KEY_A == key ||
		   GLFW_KEY_D == key )
		 && ( GLFW_PRESS == action ||
			  GLFW_REPEAT == action ) )
	{
		app.MoveCamera( key );
	}
}


void MouseCallback( GLFWwindow* window, double x, double y )
{
	static bool bFirstFocus = true;
	static float lastX = Application::Get( ).GetScreenWidth( ) / 2.0f;
	static float lastY = Application::Get( ).GetScreenHeight( ) / 2.0f;

	if ( bFirstFocus )
	{
		lastX = static_cast< float >( x );
		lastY = static_cast< float >( y );
		bFirstFocus = false;
	}

	float xOffset = static_cast< float >( x - lastX );
	float yOffset = static_cast< float >( lastY - y );

	lastX = static_cast< float >( x );
	lastY = static_cast< float >( y );

	static float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	static float yaw = -90.0f;
	static float pitch = 0.0f;

	yaw += xOffset;
	pitch += yOffset;

	pitch = std::clamp( pitch, -89.0f, 89.0f );

	glm::vec3 direction = glm::vec3{ 0.0f };
	direction.x = cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
	direction.y = sin( glm::radians( pitch ) );
	direction.z = sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );

	Application::Get( ).TurnCamera( direction );
}


void MouseScrollCallback( GLFWwindow* window, double xOffset, double yOffset )
{
	Application::Get( ).ZoomCamera( static_cast< float >( yOffset ) );
}


void WindowSizeCallback( GLFWwindow* window, int w, int h )
{
	auto& app = Application::Get( );
	app.SetViewport( w, h );
}

