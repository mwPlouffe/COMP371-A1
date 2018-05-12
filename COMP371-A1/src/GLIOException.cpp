#include "GLIOException.h"

GLIOException::GLIOException() : GLException()
{
	mError = "There was a problem that occurred when reading the shaders from file";
}
GLIOException::GLIOException(const char* message) : GLException(message)
{
	mError = message;
}
