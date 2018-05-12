#include "window.h"

//the window class implementations
//default constructor
Window::Window()
{
	mTitle = "default";
	mWidth = 800;
	mHeight = 800;
	activeManager = false;
}
//
Window::Window(const char* title, int width, int height)
{
	mTitle = title;
	mWidth = width;
	mHeight = height;
	activeManager = false;
}
Window::~Window()
{
	glfwTerminate();
}
Window::Window(const Window& other)
{
	this->mTitle =  other.mTitle;
	this->mWidth =  other.mWidth;
	this->mHeight = other.mHeight;
	this->mWindow = other.mWindow;
}
void Window::init()
{
	setErrorCallback();
	if (glfwInit() == false)
	{
		//glfwTerminate(); called implicitly when glfwinit fails
		throw GLFWException("GLFW could not be initialised");		
	}
	windowData();
	
	mWindow = glfwCreateWindow(mWidth, mHeight, mTitle, NULL, NULL);
	
	if (mWindow == NULL)
	{
		glfwTerminate();
		throw GLFWException("Could not initialise GLFWwindow");
	}
	glfwMakeContextCurrent(mWindow);
	glewExperimental = GL_TRUE;
	

	std::cout << "MESSAGE: Supported GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "MESSAGE: Renderer is: " << glGetString(GL_RENDERER) << std::endl;		

	if (glewInit() != GLEW_OK)
	{
		throw GLException("Could not initialise GLEW");
	}
	setViewPort();
}
void Window::setViewPort()
{
	int w,h;
	glfwGetFramebufferSize(mWindow,&w,&h);
	glViewport(0,0,w,h);
}
void Window::depthBuffer()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}
void Window::error_callback(int error, const char* description)
{
	std::stringstream ss{};
	ss << "ERROR CODE: " << error << " " << description;
	throw GLException( ss.str().c_str() );
}
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	bool rebind = false;
	glm::mat4 Tr;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{	
		 glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		
		Tr = glm::rotate(Tr, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		rebind = true;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		Tr = glm::rotate(Tr, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		rebind = true;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		Tr = glm::rotate(Tr, 45.0f, glm::vec3(0.0f, 0.0f, -1.0f));
		rebind = true;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		Tr = glm::rotate(Tr, 45.0f, glm::vec3(0.0f, -1.0f, 0.0f));
		rebind = true;
	}
	if (rebind)
	{
		
	}

	

}
void Window::setErrorCallback()
{
	glfwSetErrorCallback(Window::error_callback);
}
void Window::registerCallbacks()
{
	std::cout << "MESSAGE: Registering callbacks to window" << std::endl;
	glfwSetKeyCallback(mWindow, Window::key_callback);
}
void Window::enableShaders(GLuint shaderProgramID)
{
	glUseProgram(shaderProgramID);
	if (activeManager == false)
	{
		activeManager = true;
		manager = new Window::FrameManager(shaderProgramID);
		manager->init(this);
	}
}
GLuint Window::bind(glm::vec3* vertices, int vertexNum, GLenum MODE, int modeSize)
{
	//generate a vao container into which to load the object, and make it current
	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	
	GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexNum * sizeof (vertices), vertices, MODE);
	
	GLuint ebo;
	glGenBuffers(1,&ebo);
	
	//generate the indices
	glm::vec3* indices;
	
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
	

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, modeSize, GL_FLOAT, GL_FALSE, modeSize*sizeof(GLfloat), (GLvoid*) 0);
	clearContext();	
	return vao;
}
GLuint Window::rebind(GLuint vao, int vertexNum, GLenum MODE, int modeSize)
{
	return GL_FALSE;

}
void Window::drawArrays(GLuint vao, GLenum USAGE, int modeSize)
{
	manager->broadcast();
	glBindVertexArray(vao);
	glDrawArrays(USAGE, 0, modeSize);
	clearContext();
}
//this draw needs to be updated for the mesh!
void Window::drawElements(GLuint vao, GLenum USAGE, int modeSize)
{
	manager->broadcast();
	glBindVertexArray(vao);
	clearContext();
}
void Window::clearContext()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
}
void Window::windowData()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	std::cout << glfwGetVersionString() << std::endl;
}
void Window::close()
{
	glfwSetWindowShouldClose(mWindow, GL_TRUE);
}
bool Window::closed() const
{
	return glfwWindowShouldClose(mWindow);	
}
void Window::update()
{
	
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
	
}
void Window::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}
Window::FrameManager::FrameManager(glm::mat4 model, glm::mat4 view, glm::mat4 projection, GLuint shaderID)
{
	objFrameID		= glGetUniformLocation(shaderID, "model_matrix");
	cameraFrameID	= glGetUniformLocation(shaderID, "view_matrix");
	worldFrameID	= glGetUniformLocation(shaderID, "projection_matrix");
	
	modelM = model;
	viewM = view;
	projM = projection;
}
Window::FrameManager::FrameManager(GLuint shaderID)
{
	objFrameID		= glGetUniformLocation(shaderID, "model_matrix");
	cameraFrameID	= glGetUniformLocation(shaderID, "view_matrix");
	worldFrameID	= glGetUniformLocation(shaderID, "projection_matrix");
}
void Window::FrameManager::init(Window *w)
{
	//sets default matrices for the class
	int width,height;
	glfwGetFramebufferSize(w->glWindow(),&width,&height);
	projM = glm::perspective(35.0f, (GLfloat)width / (GLfloat)height, 0.0f, 300.0f);
	
	viewM = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), //camera positioned here
						glm::vec3(0.0f, 0.0f, 0.0f), //looks at origin
						glm::vec3(0.0f, 1.0f, 0.0f)); //up vector
	
	modelM = glm::scale(modelM, glm::vec3(1.0f, 1.0f, 1.0f));
}
void Window::FrameManager::updateModelMat(glm::mat4 Tr)
{
	modelM = Tr;
}
void Window::FrameManager::updateViewMat(glm::mat4 Tr)
{
	viewM = Tr;
}
void Window::FrameManager::updateProjectionMat(glm::mat4 Tr)
{
	projM = Tr;
}
void Window::FrameManager::broadcast()
{
	glUniformMatrix4fv(cameraFrameID, 1, GL_FALSE, glm::value_ptr(viewM));
	glUniformMatrix4fv(worldFrameID, 1, GL_FALSE, glm::value_ptr(projM));
	glUniformMatrix4fv(objFrameID, 1, GL_FALSE, glm::value_ptr(modelM)); //broadcast the uniform value to the shaders
}
