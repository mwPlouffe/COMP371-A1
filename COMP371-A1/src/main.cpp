#include "main.h"

using namespace cimg_library;

GLuint genEBO(GLuint vao, CImg <unsigned char> *image);

int main(int argc, const char* argv[])
{
	//placeholder path for command-line path
	const char* fpath = "./img/Heightmap.png";

	//regex validation to see if there is weird file extension
	std::string path(fpath);
	std::regex rgx("[.]([A-Za-z]{3})$");
	std::smatch sm;
	if (!std::regex_search(path, sm, rgx))
	{
		std::cout << "WARNING: The file does not appear to have a regular image extension" << std::endl;
	}
	//attempt to load the image
	CImg <unsigned char> img;
	try
	{
		img = CImg<unsigned char>(fpath);
		img.channel(0);
		//pixels = img.width() * img.height();
		
	}
	catch (CImgIOException &ex)
	{
		//all manner of badness has occurred
		//TODO the default behaviour sucks, set it in the header
		std::cout <<  "ERROR: " << ex.what() << std::endl;	
		return -1;
	}
		Window *win = new Window();
	try
	{
		win->init();
		std::cout << "MESSAGE: GLFW init successful" << std::endl;
		std::cout << "MESSAGE: GLEW init successful" << std::endl;
	}
	catch(std::exception &ex)
	{
		std::cout <<  "ERROR: " << ex.what() << std::endl;
		return -1;
	}
	ShaderProgram *sp = new ShaderProgram("./include/camera_shader.vert", "./include/shader.frag");
	
	//should wrap these in an init function
	try
	{
		sp->init();
		std::cout << "MESSAGE: Shaders init successful" << std::endl;
		
	}
	catch(GLCompilationException &ex)
	{
		std::cout <<  "ERROR: " << "Cannot recover from the exception, exiting.." <<std::endl;
		return -1;
	}
	catch(GLIOException &ex)
	{
		std::cout <<  "ERROR: " << "Cannot recover from the exception, exiting.." <<std::endl;
		return -1;
	}
	catch(GLException &ex)
	{
		std::cout <<  "ERROR: " << "Cannot recover from the exception, exiting.." <<std::endl;
		return -1;
	}
	
	win->registerCallbacks();
	std::cout << "MESSAGE: Loading image into the height map"<<std::endl;
	
	GLfloat vertices[] = {
		1.0f, 1.0f, 0.0f,  // Top
		-1.0f, 1.0f, 0.0f,  // Bottom Right
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,  // Bottom Left
	};
	GLfloat vertices1[] = {
		1.0f, 2.0f, 0.0f,  // Top
		-1.0f, 2.0f, 0.0f,  // Bottom Right
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,  // Bottom Left
	};
	
	win->enableShaders(sp->program());
	GLuint vao = win->bind(vertices, GL_STATIC_DRAW, 4, 12);
	GLuint vao1 = win->bind(vertices1, GL_STATIC_DRAW, 4, 12);
	HeightMap hmap(&img);
	win->enableManager(sp->program());
		
	
	
	while (!win->closed())
	{
		glClearColor( 0.15f, 0.15f, 0.15f, 1.0f );
		win->clear();
		win->drawArrays(vao,GL_TRIANGLE_STRIP, 8);
		win->drawArrays(vao1,GL_TRIANGLE_STRIP, 8);
		win->manager->fetch();
		win->update();
	}
	std::cout << "MESSAGE: Update loop terminated, program closing." << std::endl;
	free(win);
	free(sp);
	glfwTerminate();
	return 0;
}