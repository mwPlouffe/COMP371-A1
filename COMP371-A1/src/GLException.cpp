#include "GLException.h"

GLException::GLException()
: std::exception()
{
	mError = "An unexpected error occurred with OpenGL";
}
GLException::GLException(const char* message)
: std::exception()
{
	mError = message;
}
