#include "Macros.h"


void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogError(std::string_view functionName, std::string_view fileName, int lineNumber)
{
	unsigned int error = glGetError();

	if (GL_NO_ERROR != error)
	{
		std::cout << "[Error Code] " << error << ": " << functionName << " "
			<< fileName << "(" << lineNumber << ")" << std::endl;
		return false;
	}

	return true;
}
