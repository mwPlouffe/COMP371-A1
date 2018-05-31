#ifndef GL_COMPILATION_EXCEPTION_H
#define GL_COMPILATION_EXCEPTION_H
#include "GLException.h"

class GLCompilationException : public GLException
{
	public:
	GLCompilationException() throw();
	GLCompilationException(const char* message) throw();
};
#endif
