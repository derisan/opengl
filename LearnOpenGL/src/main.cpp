#include "Application.h"

int main()
{
	auto& app = Application::Get();
	app.Init("LearnOpenGL", 800, 600, 4, 6);
	app.Run();
	return 0;
}