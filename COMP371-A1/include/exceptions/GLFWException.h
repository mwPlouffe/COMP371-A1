#ifndef GLFW_EXCEPTION_H
#define GLFW_EXCEPTION_H
#include <exception>

class GLFWException : public std::exception
{
	protected:
		const char* mError;
		virtual const char* what() const throw()
		{
			return mError;
		};
	public:
		GLFWException() throw();
		virtual ~GLFWException() throw() {};
		GLFWException(const char* message) throw();
};
#endif
