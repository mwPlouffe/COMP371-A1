#include "main.h"

using namespace cimg_library;

int main(int argc, const char* argv[])
{
	//placeholder path for command-line path
	if (argc < 4 || argc > 5)
	{
		std::cout << " ERROR: Too few arguments provided: ./<command> <vertex shader> <fragment shader> <greyscale image> [colour image]" <<std::endl;
		return -1;
	}
	
	//command line arguments
	const char* vertexShader = "./include/shaders/camera_shader.vert";
	const char* fragmentShader ="./include/shaders/shader.frag";
	const char* hmfpath ="./img/Heightmap.png";
	const char* cfpath ="./img/output";
	
	bool isColoured = true;

	//attempt to load the height map and a corresponding colour image
	CImg <unsigned char> *img;
	CImg <unsigned char> *colour_img;
	try
	{
		img = new CImg<unsigned char>(hmfpath);
		img->channel(0);
		std::cout << "MESSAGE: Image height map successfully retreived from file" << std::endl;
		colour_img = new CImg<unsigned char>(cfpath);
		std::cout << "MESSAGE: Image colour map successfully retreived from file" << std::endl;
		
		//if the images don't match, make the user did something wrong
		if (img->width() != colour_img->width() || img->height() != colour_img->height())
		{
			throw GLIOException("The Files provided are not the same size, and cannot be used for height and colour mapping.");
		}
	}
	catch (CImgIOException &ex)
	{
		//images could not be found, cannot run the program
		//also includes assorted CImg specific IO errors
		
		std::cout <<  "EXCEPTION: " << ex.what() << std::endl;
		if (colour_img == NULL)
		{
			std::cout << "MESSAGE: Reverting to height colouring mode swapping vertex shader file" <<std::endl;
			
			vertexShader = "./include/shaders/camera_shader_height.vert";
			fragmentShader ="./include/shaders/shader_height.frag";
			isColoured = false;
		}
		else if (img == NULL)
		{
			std::cout << "ERROR: The height map file could not be found. exiting" << std::endl;
			return -1;
		}
		
		
		
	}
	catch (GLIOException &ex1)
	{
		//recover by disabling colour-mapping
		std::cout << "ERROR: " << ex1.what() << "Program will generate a heightmap with simple colouring" << std::endl;
		vertexShader = "./include/shaders/camera_shader_height.vert";
		fragmentShader ="./include/shaders/shader_height.frag";

		isColoured = false;
	}
	//instantiate the window into which the rendering is done
	Window *win = new Window("COMP371-A1 Mathew Plouffe 27532733", 800, 600);
	//try to initialise all the needed extensions for the window
	try
	{
		win->init();
		win->registerCallbacks();
		std::cout << "MESSAGE: Window init successful" << std::endl;
	}
	//catch any of the errors
	catch(std::exception &ex2)
	{
		std::cout <<  "ERROR: " << ex2.what() << std::endl;
		return -1;
	}
	//instantiate the shader
	std::cout << "MESSAGE: Instantiating the shaders from file" << std::endl;
	ShaderProgram *sp = new ShaderProgram(const_cast<char*>(vertexShader), const_cast<char*>(fragmentShader));
	try
	{
		//try to load, compile, link and detach the shaders - also deletes data after successful compilation
		sp->init();
		std::cout << "MESSAGE: Shaders init successful" << std::endl;
		sp->enable();
		std::cout << "MESSAGE: Shaders enabled." << std::endl;
	}
	//catch any exceptions, they simply halt the program
	catch(GLCompilationException &ex3)
	{
		std::cout <<  "ERROR: " << "Cannot recover from the Compilation exception, exiting.." <<std::endl;
		std::cout << ex3.what() <<std::endl;
		return -1;
	}
	catch(GLIOException &ex4)
	{
		std::cout <<  "ERROR: " << "Cannot recover from the IO exception, exiting.." <<std::endl;
		std::cout << ex4.what() <<std::endl;
		return -1;
	}
	catch(GLException &ex5)
	{
		std::cout <<  "ERROR: " << "Cannot recover from the General exception, exiting.." <<std::endl;
		std::cout << ex5.what() <<std::endl;
		return -1;
	}
	//instantiate the renderer to be able to draw to screen
	std::cout << "MESSAGE: Instantiating renderer with current Shaders" << std::endl;
	Renderer *renderer = new Renderer(sp->program());
	
	//the camera class allows for moving the view around the model
	std::cout << "MESSAGE: Initialising camera with up along y-axis" << std::endl;
	Camera *camera = new Camera(glm::vec3(0.0f,0.0f,-5.0f),
								glm::vec3(0.0f, 0.0f, 0.0f));
	
	//the renderer needs camera and window data to operate properly
	std::cout << "MESSAGE: Initialising renderer with window and camera data" << std::endl;
	renderer->init(win, camera);
	
	//grab the image data from the provided images
	std::cout << "MESSAGE: Retreiving pixel data from the colour image, and position data from greyscale" << std::endl;
	std::vector<GLfloat> positions;
	std::vector<GLfloat> pixels;
	for (int x = 0; x < img->width(); x++)
	{
		for (int z = 0; z < img->height(); z++)
		{
			positions.push_back( (GLfloat) x);
			positions.push_back( (GLfloat) (*img)(x,z,0,0));
			positions.push_back( (GLfloat) z);
			
			//allows the program to continue with default image colouring 
			if (isColoured)
			{
				pixels.push_back((GLfloat)(*colour_img)(x,z,0,0) );
				pixels.push_back((GLfloat)(*colour_img)(x,z,0,1) );
				pixels.push_back((GLfloat)(*colour_img)(x,z,0,2) );
			}
			
		}
	}
	std::cout << "MESSAGE: Instantiating a HeightMap with the colour and position data" << std::endl;
	HeightMap *map = new HeightMap(positions, pixels,img->width(), img->height());
	map->init();
	
	std::cout << "MESSAGE: Sending the HeightMap to the GPU" << std::endl;
	renderer->bind(map,GL_STATIC_DRAW);
	
	GLuint vao = renderer->bindTriangle();
	std::cout << "MESSAGE: Entering Render Loop" << std::endl;
	GLenum DRAW_MODE = GL_TRIANGLES;
	while (!win->closed())
	{
		
		renderer->clear();
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		
		if( glfwGetKey(win->glWindow(), GLFW_KEY_P) == GLFW_PRESS)
		{
			DRAW_MODE = GL_POINTS;
			glPointSize(3);
		}
		else if( glfwGetKey(win->glWindow(), GLFW_KEY_T) == GLFW_PRESS)
		{
			DRAW_MODE = GL_TRIANGLES;
		}
		else if( glfwGetKey(win->glWindow(), GLFW_KEY_W) == GLFW_PRESS)
		{
			DRAW_MODE = GL_LINES;
			glLineWidth(3);
		}
		renderer->drawElements(vao, GL_TRIANGLES, 6);
		renderer->drawElements(map->vao, DRAW_MODE, map->indexSize());
		
		renderer->transformModelMatrix(
									   camera->updateModel(win->keyPressed));
		renderer->update(win);
	}
	//free resourses and exit gracefully
	std::cout << "MESSAGE: Update loop terminated, program closing." << std::endl;
	free(win);
	free(sp);
	free(renderer);
	free(map);
	glfwTerminate();
	return 0;
}