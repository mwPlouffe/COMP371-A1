#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <sstream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLException.h"
#include "GLFWException.h"

#define MAX_KEYS 13

class Window
{
	//members
	private:	
		const char* mTitle;
		int mWidth, mHeight;
		GLFWwindow* mWindow;
	private:
	//private helper functions
		void windowData(void);
	public:
	//constructors and destructors
		static bool keyPressed[MAX_KEYS];		//used to grab the key presses
		~Window(void);
		Window(const char* title, int width, int height);
	//utility
		inline std::string title(void) 	 const 	{return mTitle;};
		inline int	   width(void)		{return mWidth;};
		inline int 	   height(void)		{return mHeight;};
		inline GLFWwindow* glWindow(void)	{return (mWindow != NULL) ? mWindow: GL_FALSE;};
	//gl functionalities
		void init(void);
		void close(void);
		bool closed(void) const;
		void setViewPort(void);
	
	//callbacks and error handling
		static void error_callback(int error, const char* description);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		void setErrorCallback(void);
		void registerCallbacks(void);
};


#endif 

