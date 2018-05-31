#include "GLCompilationException.h"

//GL compilation exception class, inherites from GL exception

GLCompilationException::GLCompilationException() throw()
: GLException()
{
	mError = "There a problem when compiling an OpenGL shader";
}
GLCompilationException::GLCompilationException(const char* message) throw()
: GLException(message)
{
	mError = message;
}
