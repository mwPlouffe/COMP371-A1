//
//  texture.cpp
//  COMP371-A2
//
//  Created by Mathew Plouffe on 2018-05-16.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "texture.h"

using namespace cimg_library;

Texture::Texture(const char* fp, GLenum DIMS, GLenum COL)
{
	texturePath = fp;
	DIM_TYPE = DIMS;
	COL_TYPE = COL;
}
void Texture::init(void)
{
	if (load() == false)
	{
		throw GLIOException("ERROR: Texture not found");
	}
	
	glGenTextures(1, &tex);
	glBindTexture(DIM_TYPE, tex);
	
	//set the texture blending options - this one performs linear interpolation on the textures
	glTexParameteri(DIM_TYPE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(DIM_TYPE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//allows for 1D and 2D textures to be used with th
	if (DIM_TYPE == GL_TEXTURE_1D)
	{
		glTexImage1D(DIM_TYPE, 0, COL_TYPE, texture.width(), 0, 0, COL_TYPE, texture.data() );
		glGenerateMipmap(DIM_TYPE);
	}
	else if (DIM_TYPE == GL_TEXTURE_2D)
	{
		glTexImage2D(DIM_TYPE, 0, COL_TYPE, texture.width(), texture.height(), 0, COL_TYPE, GL_UNSIGNED_BYTE, texture.data() );
		glGenerateMipmap(DIM_TYPE);
	}
	else
	{
		throw GLException("EXCEPTION: Texture could not be loaded, it does not have the correct number of dimensions.");
	}
	glBindTexture(DIM_TYPE, 0);
	free(texture);
}
bool Texture::load(void)
{
	cimg_library::cimg::exception_mode(0);
	try
	{
		texture = *new CImg<unsigned char> (texturePath);
	}
	catch (CImgIOException &ex)
	{
		std::cout << "EXCEPTION: " << ex.what() << std::endl;
		return false;
	}
	free(&texturePath);
	return true;
}