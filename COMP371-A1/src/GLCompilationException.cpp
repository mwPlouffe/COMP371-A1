#include "GLCompilationException.h"

GLCompilationException::GLCompilationException() : GLException()
{
	mError = "There a problem when compiling an OpenGL shader";
}
GLCompilationException::GLCompilationException(const char* message) : GLException(message)
{
	mError = message;
}
