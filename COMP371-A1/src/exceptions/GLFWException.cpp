#include "GLFWException.h"

//GLFW exception class, inherites from std::exception


GLFWException::GLFWException() throw()
: std::exception()
{
	mError = "There was a problem that occurred when using GLFW";
}
GLFWException::GLFWException(const char* message) throw()
: std::exception()
{
	mError = message;
}
