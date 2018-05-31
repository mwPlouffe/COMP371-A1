#include "window.h"

bool Window::keyPressed[MAX_KEYS];

//the window class implementations
//default constructor
Window::Window(const char* title, int width, int height)
{
	mTitle = title;
	mWidth = width;
	mHeight = height;
}
Window::~Window()
{
	glfwTerminate();
}
void Window::init()
{
	//this function ensures that the supporting libraries can be initialised properly
	//the exceptions and messages seem rather explanatory
	
	setErrorCallback();
	
	//init glfw, else throw exception
	if (glfwInit() == false)
	{
		throw GLFWException("GLFW could not be initialised");
	}
	std::cout << "\tMESSAGE: GLFW init successful" << std::endl;
	
	//window data forces the OGL version to be modern, along with displaying useful debug info
	windowData();
	
	mWindow = glfwCreateWindow(mWidth, mHeight, mTitle, NULL, NULL);
	
	if (mWindow == NULL)
	{
		glfwTerminate();
		throw GLFWException("Could not initialise GLFWwindow");
	}
	
	glfwMakeContextCurrent(mWindow);
	glewExperimental = GL_TRUE;
	
	//more useful information, can only be displayed after glfw and OGL are initialised successfully
	std::cout << "MESSAGE: Supported GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "MESSAGE: Renderer is: " << glGetString(GL_RENDERER) << std::endl;
	
	if (glewInit() != GLEW_OK)
	{
		throw GLException("Could not initialise GLEW");
	}
	std::cout << "\tMESSAGE: GLEW init successful" << std::endl;
	setViewPort();
}
void Window::setViewPort()
{
	//set the size of the window
	int w,h;
	glfwGetFramebufferSize(mWindow,&w,&h);
	glViewport(0,0,w,h);
	std::cout << "\tMESSAGE: ViewPort Size for Window is set" << std::endl;
}
void Window::windowData()
{
	
	//force the OGL version to be OGL 3.3 (modern), as well as the GLSL language type to support
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//the window is not resizable as a result, don't even try
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	//output the version to the user
	std::cout << "MESSAGE: " << glfwGetVersionString() << std::endl;
	
	
}
void Window::error_callback(int error, const char* description)
{
	//the error callback, it *should* catch any problems with OGL and send a flare up
	std::stringstream ss{};
	ss << "ERROR CODE: " << error << " " << description;
	throw GLException( ss.str().c_str() );
}
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//the key bindings, this setup allows for hold-presses of buttons
	//see the README for what the key bindings do
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		keyPressed[0] = true;
	}
	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
	{
		keyPressed[0] = false;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		keyPressed[1] = true;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
	{
		keyPressed[1] = false;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		keyPressed[2] = true;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
	{
		keyPressed[2] = false;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		keyPressed[3] = true;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
	{
		keyPressed[3] = false;
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		keyPressed[4] = true;
	}
	if (key == GLFW_KEY_X && action == GLFW_RELEASE)
	{
		keyPressed[4] = false;
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		keyPressed[5] = true;
	}
	if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
	{
		keyPressed[5] = false;
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		keyPressed[6] = true;
	}
	if (key == GLFW_KEY_K && action == GLFW_RELEASE)
	{
		keyPressed[6] = false;
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		keyPressed[7] = true;
	}
	if (key == GLFW_KEY_J && action == GLFW_RELEASE)
	{
		keyPressed[7] = false;
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		keyPressed[8] = true;
	}
	if (key == GLFW_KEY_I && action == GLFW_RELEASE)
	{
		keyPressed[8] = false;
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		keyPressed[9] = true;
	}
	if (key == GLFW_KEY_L && action == GLFW_RELEASE)
	{
		keyPressed[9] = false;
	}
	
	//render mode bindings that are unused. It is silly to have the user constantly hold down a button to render in a particular mode
/*	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		keyPressed[10] = true;
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		keyPressed[10] = false;
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		keyPressed[11] = true;
	}
	if (key == GLFW_KEY_P && action == GLFW_RELEASE)
	{
		keyPressed[11] = false;
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		keyPressed[12] = true;
	}
	if (key == GLFW_KEY_T && action == GLFW_RELEASE)
	{
		keyPressed[12] = false;
	}*/

}
void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//TODO implement
	//not needed for the assignment
}
void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//TODO implement
	//not needed for the first assignment
}
void Window::setErrorCallback()
{
	//the error callback can be registered without a window or a context, so it is registered soonest
	std::cout << "\tMESSAGE: GLFW error callback attached" << std::endl;
	glfwSetErrorCallback(Window::error_callback);
}
void Window::registerCallbacks()
{
	//register all the other callbacks to the window
	std::cout << "\tMESSAGE: Registering callbacks to window" << std::endl;
	glfwSetKeyCallback(glWindow(), Window::key_callback);
	glfwSetMouseButtonCallback(glWindow(), Window::mouse_button_callback);
	glfwSetCursorPosCallback(glWindow(), Window::cursor_position_callback);
}
void Window::close()
{
	//tells the window to close
	glfwSetWindowShouldClose(mWindow, GL_TRUE);
}
bool Window::closed() const
{
	//boolean to poll if the window should close
	return glfwWindowShouldClose(mWindow);
}
