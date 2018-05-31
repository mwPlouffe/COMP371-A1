#ifndef MAIN_H
	#define MAIN_H

	//language standard libs
	#include <iostream>
	#include <regex>
	#include <string>
	#include <unistd.h>
		
	//**CImg library**//
	
	//random image support settings
	#define cimg_use_png
	#define cimg_use_jpeg
	#define cimg_use_tiff
	
	//extra support for image operations, likely to be removed
	//#define cimg_use_magick

	#if !defined(cimg_display) || (cimg_display != 0) 
		//removes the X11 capabilities for the library, not needed
		//needs to be set before inclusion of CImg.h, since it compiles with program its used in
		#define cimg_display 0
	#endif
	#include "CImg.h"
	
	//**ImageMagick library**//
	//provides support for some of the CImg functionalities
	//#include "Magick++.h"

	//opengl and supported libraries
	//farmed out to other classes
	#include "GL/glew.h"
	#include "GLFW/glfw3.h"
	#include "glm/glm.hpp"
	#include <glm/gtc/matrix_transform.hpp>
		
	//my defined classes 
	#include "window.h"
	#include "shaderProgram.h"
	#include "renderer.h"
	#include "camera.h"
	#include "heightMap.h"

#endif


