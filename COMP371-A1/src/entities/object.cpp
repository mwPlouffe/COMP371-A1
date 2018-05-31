//
//  object.cpp
//  COMP371-A1
//
//  Created by Mathew Plouffe on 2018-05-14.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "object.h"

//the base type of everything to be rendered in the OGL context
//provides virtual functions for all objects to use dynamically
//allows for only one render and bind function to be used by the renderer

Object::Object(void)
{
	//intentionally empty
}
Object::Object(std::vector<GLfloat> vertexList,std::vector<GLuint> indexList)
{
	vertices = vertexList;
	indices = indexList;
}
Object::Object(std::vector<GLfloat> vertexList)
{
	vertices = vertexList;
}
Object::Object(std::vector<GLfloat> vertexList,std::vector<GLfloat> colourList)
{
	vertices = vertexList;
	colours	= colourList;
}
std::vector<GLfloat> Object::vertexList(void)
{
	return vertices;
}
std::vector<GLfloat> Object::colourList(void)
{
	return colours;
}
//polymorphic, generates a simple ibo that has each vertex being a index
std::vector<GLuint> Object::pointIndex(void)
{
	if (indices.size() != 0)
	{
		return indices;
	}
	int index = 0;
	for (int i = 0; i < vertices.size(); i+=3)
	{
		indices.push_back(index++);
		//std::cout << "(" << vertices[i] << ", " << vertices[i+1] << ", " << vertices[i+2] << ")" << std::endl;
	}
	return indices;
}

//polymorphics, generates the ibo data
std::vector<GLuint> Object::triangleIndex(void)
{
	//intentially left unimplemented, simply returns the points index values
	return indices;
}

//polymorphic
//populates the ibo data
void Object::init()
{
	pointIndex();
}
