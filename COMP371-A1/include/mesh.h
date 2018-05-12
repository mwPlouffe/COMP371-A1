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
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		GLuint index;
		Vertex(void);
		Vertex(glm::vec3 pos);
		Vertex(glm::vec3 pos, int i);
		Vertex(glm::vec3 pos, glm::vec3 norm);
		
		//this operator compares floating point numbers, be careful with its use
		bool operator==(const Vertex &v);
		bool equals(const Vertex &v);
		float squareDist(const Vertex &v) const;
		float squareSum(void) const;
	};
	struct Edge
	{
		Vertex *start;
		Vertex *end;
		bool valid;
		Edge(void);
		Edge(Vertex *s, Vertex *e);
		float slope(void);
		void flip(void);
		
		//this operator compares floating point numbers, be careful with its use
		//compares the distance of all the coordinates involved, NOT a content equals operator.
		bool operator==(const Edge &e);
		
	};
	struct Triangle
	{
			Edge *e_ab;
			Edge *e_bc;
			Edge *e_ca;
			bool valid;
			Triangle(void);
			Triangle(Vertex* a, Vertex* b, Vertex* c);
			bool contains(const Vertex& v);
			bool envelopes(const Vertex &v);
			void reorder(void);
	};
	
		friend struct Vertex;
		friend struct Triangle;
	private:
		std::vector<Vertex>		vertices;		//the points to be used in the mesh
		std::vector<Triangle>	triangles;	//keeps track of all the triangle primitives
		std::vector<Edge>		edges;		//holds the edges of all the triangle primitives
		std::vector<GLuint>		indices;	//holds the index of the vertices, in the order to be rendered
		GLuint vbo ,ebo;
	public:
		GLuint vao;
		Mesh(glm::vec3 *v, size_t size);
		GLuint bind(void);				//implicitly clears context
		GLuint clearContext(void);
	private:
		void init(void);
		void index(void);
		Triangle& superTriangle(const float midX, const float midY, const float delta);
		float* bounds(float minX, float minY);
		void markVertices(const Vertex& v);
		void checkEdges(void);
		void GenerateTriangles(const Vertex& v);
		void indexList(void);
		std::vector<glm::vec3> vertexList(void);
	
	

};

#endif /* mesh_h */
