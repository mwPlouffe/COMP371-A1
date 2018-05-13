//
//  mesh.h
//  COMP371-A1
//
//  Created by Mathew Plouffe on 2018-05-11.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef mesh_h
#define mesh_h
#include <vector>
#include <iostream>
#include <algorithm>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLException.h"
#include "GLFWException.h"

#define MAX_DIST_MODIFIER 30

class Mesh
{
	protected: struct Vertex
	{
		glm::vec3 position;
		//glm::vec3 normal;
		GLuint index;
		Vertex(void);
		Vertex(glm::vec3 pos);
		Vertex(glm::vec3 pos, int i);
		Vertex(glm::vec3 pos, glm::vec3 norm);
		
		//compares the ID of each vertex
		bool operator==(const Vertex &v);
	};
	protected: struct Edge
	{
		Vertex *start;
		Vertex *end;
		bool valid;
		Edge(void);
		Edge(Vertex *s, Vertex *e);
		float slope(void);
		void flip(void);
		
		//compares the start and end IDs to see if the two vertices of each edge are the same (position independent)
		bool operator==(const Edge &e);
		
	};
	protected: struct Triangle
	{
			Edge *e_ab;
			Edge *e_bc;
			Edge *e_ca;
			bool valid;
			Triangle(void);
			Triangle(Vertex* a, Vertex* b, Vertex* c);
			void reorder(void);
	};
	
		friend struct Vertex;
		friend struct Triangle;
	protected:
		std::vector<Vertex>		vertices;		//the points to be used in the mesh
		std::vector<Triangle>	triangles;	//keeps track of all the triangle primitives
		std::vector<Edge>		edges;		//holds the edges of all the triangle primitives
		std::vector<GLuint>		indices;	//holds the index of the vertices, in the order to be rendered
		GLuint vbo ,ebo;
	public:
		GLuint vao;
		Mesh(void);
		Mesh(glm::vec3 *v, size_t size);
		GLuint clearContext(void);
		GLuint bind(void);					//implicitly clears context
		inline int renderSize(void)
		{
			return indices.size();
		};
	protected:
		virtual void index(void);
		void init(void);
		virtual void indexList(void);
		std::vector<glm::vec3> vertexList(void);	//unwraps the vertices for passing as a vbo argument
};

#endif /* mesh_h */
