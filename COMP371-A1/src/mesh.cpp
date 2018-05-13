//
//  mesh.cpp
//  COMP371-A1
//
//  Created by Mathew Plouffe on 2018-05-11.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "mesh.h"

///////////////////////////////////////
//MESH implementations
/////////////////////////////////////////

Mesh::Mesh(void)
{
	//intentionally empty, inherited by children
}

Mesh::Mesh(glm::vec3 *v, size_t size)
{
	//add the passed vec3's into vertices in the vertex wrapper
	for (int i = 0; i < size; i++)
	{
		vertices.push_back(Vertex(v[i],i) );
		//std::cout <<"Vertex " << vertices[i].index << ": (" << vertices[i].position.x << ", " << vertices[i].position.y << ", " << vertices[i].position.z << ")"<< std::endl;
		
	}
	//generate a vao
	init();
	
	//index the mesh to properly render the triangles
	index();
	
	//generate a position vbo, and an ebo for the mesh, and sends them to be processed
	bind();
}
GLuint Mesh::bind(void)
{
	//glGenBuffers(1,&ebo);
	//indexList();
	//glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW );
	
	glGenBuffers(1,&vbo);
	std::vector<glm::vec3> vert = vertexList();
	
	glBufferData( GL_ARRAY_BUFFER, vert.size() * 3, &vert[0], GL_STATIC_DRAW );
	
	clearContext();
	return vao;
}
GLuint Mesh::clearContext(void)
{
	glBindVertexArray(0);
	return vao;
}
void Mesh::init(void)
{
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
}
void Mesh::indexList()
{
	std::cout<<"making the indices" << std::endl << std::endl;
	indices = std::vector<GLuint>();
	for (int i = 0; i < triangles.size(); i++)
	{
		indices.push_back(triangles[i].e_ab->start->index);
		indices.push_back(triangles[i].e_bc->start->index);
		indices.push_back(triangles[i].e_ca->start->index);
		std::cout <<"Triangle "  << i << ": Vertex Indexes("<<  triangles[i].e_ab->start->index << ", " << triangles[i].e_bc->start->index << ", " << triangles[i].e_ca->start->index << ")" << std::endl;
	}
}
std::vector<glm::vec3> Mesh::vertexList()
{
	std::cout<<"returning the vertices" << std::endl << std::endl;
	std::vector<glm::vec3> vert;
	for (int i = 0; i < vertices.size(); i++)
	{
		vert.push_back (vertices[i].position);
		//std::cout <<"Vertex " << vertices[i].index << ": (" << vertices[i].position.x << ", " << vertices[i].position.y << ", " << vertices[i].position.z << ")"<< std::endl;
	}
	return vert;
}
void Mesh::index(void)
{
	
	//TODO provide a general mesh implementation
	
}
///////////////////////////////////////
//VERTEX implementations
/////////////////////////////////////////
Mesh::Vertex::Vertex()
{
	position = glm::vec3();
	//normal = glm::vec3();
}
Mesh::Vertex::Vertex(glm::vec3 pos)
{
	position = pos;
	//normal = glm::vec3();
}
Mesh::Vertex::Vertex(glm::vec3 pos, int i)
{
	position = pos;
	index = i;
}
Mesh::Vertex::Vertex(glm::vec3 pos, glm::vec3 norm)
{
	position = pos;
	//normal = norm;
}
bool Mesh::Vertex::operator==(const Vertex &v)
{
	return (this->index == v.index);
}
///////////////////////////////////////
//EDGE implementations
/////////////////////////////////////////
Mesh::Edge::Edge()
{
	start = new Vertex();
	end = new Vertex();
	valid = true;
}
Mesh::Edge::Edge(Vertex* s, Vertex* e)
{
	start = s;
	end = e;
	valid = true;
}
bool Mesh::Edge::operator==(const Edge &e)
{
	return (	(this->start == e.start && this->end == e.end)
			||	(this->start == e.end	&& this->end == e.start)
			);
}
void Mesh::Edge::flip()
{
	Vertex** tmp = &start;
	start = end;
	end = *tmp;
}
float Mesh::Edge::slope()
{
	return (start->position.x - end->position.x) * (start->position.y - end->position.y);
}
///////////////////////////////////////
//TRIANGLE implementations
/////////////////////////////////////////
Mesh::Triangle::Triangle()
{
	e_ab = new Edge();
	e_bc = new Edge();
	e_ca = new Edge();
	valid = true;
	reorder();
}
Mesh::Triangle::Triangle(Mesh::Vertex* a, Mesh::Vertex* b, Mesh::Vertex* c)
{
	e_ab = new Edge(a, b);
	e_bc = new Edge(b, c);
	e_ca = new Edge(c, a);
	valid = true;
	reorder();
}
void Mesh::Triangle::reorder()
{
	float dir = e_ab->slope() + e_bc->slope() + e_ca->slope();
	
	//defined in a CW manner, needs to be CCW
	if (dir > 0)
	{
		e_ab->flip();
		e_bc->flip();
		e_ca->flip();
	}
}














