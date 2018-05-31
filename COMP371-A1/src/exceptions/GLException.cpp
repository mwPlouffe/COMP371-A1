#include "GLException.h"

//GL compilation exception class, inherites from std::exception


GLException::GLException() throw()
: std::exception()
{
	mError = "An unexpected error occurred with OpenGL";
}
GLException::GLException(const char* message) throw()
: std::exception()
{
	mError = message;
}
