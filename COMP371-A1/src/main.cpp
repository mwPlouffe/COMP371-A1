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
	int pixels = 4;
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
	//	glm::vec3* vertices1 = (glm::vec3*) malloc (sizeof(glm::vec3) * pixels * pixels);
	
	glm::vec3 vertices[] =
	{
		glm::vec3(0.5f,  0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(-0.5f,  0.5f, 0.0f),
	};
	int index = 0;
	std::cout << "MESSAGE: Loading image";
//	for (int i = pixels -1; i >= 0; i--)
//	{
//		for (int j = pixels - 1; j >= 0; j--)
//		{
//			std::cout << "Pixel " << index << ": (" << i << ", " << j << ", " << (int) img(i,j,0,0) << ")" << std::endl;
//			index =(pixels * pixels) - (j + i * pixels) - 1;
//			vertices1[index] = *new glm::vec3(i, j, (int)img(i,j,0,0) );
//		}
//	}
	std::cout<<std::endl << "MESSAGE: Done Loading image" << std::endl;
	Mesh* mesh = new Mesh(vertices, 4);

	std::cout << "MESSAGE: Binding vertices to vao" << std::endl;
	win->enableShaders(sp->program());
	std::cout << "MESSAGE: entering update loop" << std::endl;
	while ( ! win->closed() )
	{
		win->clear();
		win->drawElements(mesh->vao, GL_TRIANGLES, 3);
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		win->update();
	}
	std::cout << "MESSAGE: Update loop terminated, program closing." << std::endl;
	free(win);
	free(sp);
	glfwTerminate();
	return 0;
}