#include "Application.h"

int main( )
{
	auto app = std::make_unique<Application>( "LearnOpenGL", 800, 600, 4, 6 );
	app->Run( );

	return 0;
}