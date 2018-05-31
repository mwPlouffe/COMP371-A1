//
//  heightMap.cpp
//  COMP371-A1
//
//  Created by Mathew Plouffe on 2018-05-12.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "heightMap.h"

//inherits from object
//generates a map from the provided data, as well as the width and height of the map

HeightMap::HeightMap(void) : Object()
{
	//intentionally left empty
}
HeightMap::HeightMap(std::vector<GLfloat> vertices, int w, int h)
:Object(vertices)
{
	width = w;
	height = h;
}
HeightMap::HeightMap(std::vector<GLfloat> vertices,std::vector<GLfloat> colours, int w, int h)
: Object(vertices, colours)
{
	width = w;
	height = h;
}

//poylmorphic
//generates the ibo data required to render the surface using triangles
std::vector<GLuint>HeightMap::triangleIndex(void)
{
	//used the algorithm presented in this tutorial for the triangle generation
	//https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/indexbuffers/Tutorial%208%20-%20Index%20Buffers.pdf
	
	for (int x = 0; x < width - 1; x++)
	{
		for (int z = 0; z < height - 1; z++)
		{
			GLuint a =( x		* (height)) +  z;
			GLuint b =((x+1)	* (height)) +  z;
			GLuint c =((x+1)	* (height)) + (z + 1);
			GLuint d =( x		* (height)) + (z + 1);
			
			indices.push_back(a);
			indices.push_back(d);
			indices.push_back(b);
			//std::cout << "(" << c << ", " << b << ", " << a << ")" << "\t(" << x << ", " << z << ")" << std::endl;
			indices.push_back(b);
			indices.push_back(d);
			indices.push_back(c);
			//std::cout << "(" << c << ", " << a << ", " << d << ")" << "\t(" << x << ", " << z << ")" << std::endl;
		}
	}
	return indices;
}

//returns the Base object version point index, not need to do anything else
std::vector<GLuint>HeightMap::pointIndex(void)
{
		return Object::pointIndex();
}

//populates the ibo data before it is bound
void HeightMap::init(void)
{
	triangleIndex();
}