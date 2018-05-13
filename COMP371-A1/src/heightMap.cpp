//
//  heightMap.cpp
//  COMP371-A1
//
//  Created by Mathew Plouffe on 2018-05-12.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "heightMap.h"

HeightMap::HeightMap(CImg<unsigned char> *image)
{
	width = image->width();
	height = image->height();
	init();
	generateVertices(*image);
	index();
	bind();
}
void HeightMap::generateVertices(CImg<unsigned char> image)
{
	int index = 0;
	for (int k = 0; k < width; k++)
	{
		for (int i = 0; i < height; i++)
		{
			//std::cout << "Pixel " << index << ": (" << i << ", " << j << ", " << (int) image(i,k,0,0) << ")" << std::endl;
			vertices.push_back( Vertex(glm::vec3(k, image(k,i,0,0), i), index) );
			index = i + (k * width) + 1;
			
		}
	}
}
void HeightMap::index()
{	
	int counter = 0;
	int disp = 0;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int i0 = ( j	* width) +  i;
			int i1 = ((j+1) * width) +  i;
			int i2 = ((j+1) * width) + (i + 1);
			int i3 = ( j	* width) + (i + 1);
			
			indices.push_back(i2);
			indices.push_back(i1);
			indices.push_back(i0);
			std::cout <<"Triangle "  << j + (i * width) +disp++ << ": Vertex Indexes("<<  i2 << ", " << i1 << ", " << i0 << ")" << std::endl;
			counter+=3;
			indices.push_back(i0);
			indices.push_back(i3);
			indices.push_back(i2);
			std::cout <<"Triangle "  << j + (i * width) +disp++ << ": Vertex Indexes("<<  i0 << ", " << i3 << ", " << i2 << ")" << std::endl;
			counter+=3;
		}
	}
	
}
void HeightMap::indexList(void)
{
	//intentionally empty
}




