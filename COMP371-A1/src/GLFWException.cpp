#include "GLFWException.h"

GLFWException::GLFWException() : std::exception()
{
	mError = "There was a problem that occurred when using GLFW";
}
GLFWException::GLFWException(const char* message) : std::exception()
{
	mError = message;
}
