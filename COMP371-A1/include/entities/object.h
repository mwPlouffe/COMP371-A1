//
//  object.h
//  COMP371-A1
//
//  Created by Mathew Plouffe on 2018-05-14.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef object_h
#define object_h

#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Object
{
	protected:
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	std::vector<GLfloat> colours;
	GLuint tex;
	public:
		GLuint vao;
		Object(void);
		Object(std::vector<GLfloat> vertexList,std::vector<GLuint> indexList);
		Object(std::vector<GLfloat> vertexList,std::vector<GLfloat> colourList);
		Object(std::vector<GLfloat> vertexList);
		std::vector<GLfloat> vertexList(void);
		std::vector<GLfloat> colourList(void);
		inline int indexSize(void)
		{
			return indices.size();
		}
		inline int vertexSize(void)
		{
			return vertices.size();
		}
		inline int colourSize(void)
		{
			return colours.size();
		}
		virtual std::vector<GLuint> triangleIndex(void);
		virtual std::vector<GLuint> pointIndex(void);
		virtual void init(void);
};


#endif /* object_h */
