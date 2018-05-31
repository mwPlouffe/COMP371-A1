#ifndef GL_EXCEPTION_H
#define GL_EXCEPTION_H

#include <exception>

class GLException : public std::exception
{
	//members
	protected:
	const char* mError;
	
	//constructors
	public:
		GLException() throw();
		GLException(const char* message) throw();
		virtual ~GLException() throw(){};
		virtual const char* what()
		{
			return mError;
		}
		
};
#endif
