#ifndef GL_IO_EXCEPTION_H
#define GL_IO_EXCEPTION_H
#include "GLException.h"

class GLIOException : public GLException
{
	public:
	GLIOException() throw();
	GLIOException(const char* message) throw();
};
#endif
