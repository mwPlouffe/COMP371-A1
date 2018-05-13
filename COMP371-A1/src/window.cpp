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
	//depthBuffer();
}
void Window::setViewPort()
{
	int w,h;
	glfwGetFramebufferSize(mWindow,&w,&h);
	glViewport(0,0,w,h);
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
void Window::depthBuffer()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}
void Window::error_callback(int error, const char* description)
{
	std::stringstream ss{};
	ss << "ERROR CODE: " << error << " " << description;
	throw GLException( ss.str().c_str() );
}
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{	
		 glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		glm::mat4 Rx;
		FrameManager::pushModelMat( glm::rotate_slow(Rx, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f)) );
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		glm::mat4 Rxm;
		FrameManager::pushModelMat( glm::rotate_slow(Rxm, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f)) );
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		glm::mat4 Rz;
		FrameManager::pushModelMat( glm::rotate_slow(Rz, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f)) );
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		glm::mat4 Rzm;
		FrameManager::pushModelMat( glm::rotate_slow(Rzm, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, -1.0f)) );
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
	std::cout << "MESSAGE: Shaders Enabled"<<std::endl;
	glUseProgram(shaderProgramID);
}
void Window::enableManager(GLuint shaderProgramID)
{
	if (activeManager == false)
	{
		std::cout << "MESSAGE: FrameManager initialised"<<std::endl;
		activeManager = true;
		manager = new Window::FrameManager(shaderProgramID);
		manager->init(this);
	}
}
GLuint Window::bind(GLfloat *vertices, GLenum DRAW_MODE, int vertexNum, int elements)
{
	std::cout << "MESSAGE: Binding vertices to Buffer"<<std::endl;
	GLuint vao,vbo;
	glGenVertexArrays(1,&vao);
	glGenBuffers(1,&vbo);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, elements * sizeof(vertices), vertices, DRAW_MODE);
	
	glVertexAttribPointer(0, vertexNum, GL_FLOAT, GL_FALSE, vertexNum * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	clearContext();
	return vao;
}
void Window::drawArrays(GLuint vao, GLenum USAGE, int modeSize)
{
		manager->broadcast();
		glBindVertexArray(vao);
		glDrawArrays(USAGE, 0, modeSize);
		clearContext();
}
void Window::drawElements(Mesh *m, GLenum USAGE)
{
	//use this to draw things by the element index object
}
void Window::clearContext()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
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


glm::mat4 Window::FrameManager::modelMT;
glm::mat4 Window::FrameManager::viewMT;
glm::mat4 Window::FrameManager::projMT;
bool Window::FrameManager::set;

Window::FrameManager::FrameManager(glm::mat4 model, glm::mat4 view, glm::mat4 projection, GLuint shaderID)
{
	objFrameID		= glGetUniformLocation(shaderID, "model_matrix");
	cameraFrameID	= glGetUniformLocation(shaderID, "view_matrix");
	worldFrameID	= glGetUniformLocation(shaderID, "projection_matrix");
	
	modelM = model;
	viewM = view;
	projM = projection;
	
	if (!FrameManager::set)
	{
		FrameManager::set = true;
		FrameManager::modelMT = glm::mat4();
		FrameManager::viewMT = glm::mat4();
		FrameManager::projMT = glm::mat4();
	}
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
	projM = glm::perspective(35.0f, (GLfloat)width / (GLfloat)height, 0.0f, 100.0f);
	
	viewM = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), //camera positioned here
						glm::vec3(0.0f, 0.0f, 0.0f), //looks at origin
						glm::vec3(0.0f, 1.0f, 0.0f)); //up vector
	
	modelM = glm::scale(modelM, glm::vec3(1.0f, 1.0f, 1.0f));
}
void Window::FrameManager::updateModelMat(glm::mat4 Tr)
{
	modelM =Tr * modelM;
}
void Window::FrameManager::updateViewMat(glm::mat4 Tr)
{
	viewM = Tr * viewM;
}
void Window::FrameManager::updateProjectionMat(glm::mat4 Tr)
{
	projM = Tr * projM;
}
void Window::FrameManager::broadcast()
{
	glUniformMatrix4fv(cameraFrameID, 1, GL_FALSE, glm::value_ptr(viewM));
	glUniformMatrix4fv(worldFrameID, 1, GL_FALSE, glm::value_ptr(projM));
	glUniformMatrix4fv(objFrameID, 1, GL_FALSE, glm::value_ptr(modelM)); //broadcast the uniform value to the shaders
}
void Window::FrameManager::fetch()
{
	//grab the changes, then reset the changes
	updateModelMat(FrameManager::modelMT);
	updateProjectionMat(FrameManager::projMT);
	updateViewMat(FrameManager::viewMT);
	FrameManager::modelMT = glm::mat4();
	FrameManager::viewMT = glm::mat4();
	FrameManager::projMT = glm::mat4();
	
}
void Window::FrameManager::pushModelMat(glm::mat4 Tr)
{
	FrameManager::modelMT = Tr * FrameManager::modelMT;
}
void Window::FrameManager::pushViewMat(glm::mat4 Tr)
{
	FrameManager::viewMT = Tr * FrameManager::viewMT;
}
void Window::FrameManager::pushProjectionMat(glm::mat4 Tr)
{
	FrameManager::projMT = Tr * FrameManager::projMT;
}
