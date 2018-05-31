#include "GLIOException.h"

//GL IO exception class, inherites from GL exception


GLIOException::GLIOException() throw()
: GLException()
{
	mError = "There was a problem that occurred when reading the shaders from file";
}
GLIOException::GLIOException(const char* message) throw()
: GLException(message)
{
	mError = message;
}
