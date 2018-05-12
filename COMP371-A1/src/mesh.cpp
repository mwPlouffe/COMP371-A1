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


Mesh::Mesh(glm::vec3 *v, size_t size)
{
	//add the passed vec3's into vertices in the vertex wrapper
	for (int i = 0; i < size; i++)
	{
		vertices.push_back(Vertex(v[i],i) );
		std::cout <<"Vertex " << vertices[i].index << ": (" << vertices[i].position.x << ", " << vertices[i].position.y << ", " << vertices[i].position.z << ")"<< std::endl;
		
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
	glGenBuffers(1,&ebo);
	indexList();
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW );
	
	glGenBuffers(1,&vbo);
	std::vector<glm::vec3> vert = vertexList();
	
	glBufferData( GL_ARRAY_BUFFER, vert.size() * 3, &vert[0], GL_STATIC_DRAW );
	
	clearContext();
	return vao;
}
GLuint Mesh::clearContext(void)
{
	glBindVertexArray( 0 );
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
		std::cout <<"Vertex " << vertices[i].index << ": (" << vertices[i].position.x << ", " << vertices[i].position.y << ", " << vertices[i].position.z << ")"<< std::endl;
	}
	return vert;
}
Mesh::Triangle& Mesh::superTriangle(const float midX, const float midY, const float delta)
{
	//generate the super triangle with sufficiently large values that it covers all of the points
	//the super triangle "seeds" the data, and should not be rendered.
	Triangle *ret =new Triangle(
						 new Vertex( glm::vec3(midX - (MAX_DIST_MODIFIER * delta), midY - delta,						0.0f)),
						 new Vertex( glm::vec3(midX,							 midY + MAX_DIST_MODIFIER * delta,  0.0f)),
						 new Vertex( glm::vec3(midX + (MAX_DIST_MODIFIER * delta), midY - delta,						0.0f))
						 );
	
	ret->e_ab->start->index = vertices.size() + 1;
	ret->e_bc->start->index = vertices.size() + 2;
	ret->e_ca->start->index = vertices.size() + 3;
	
	return *ret;
}
float* Mesh::bounds(float minX, float minY)
{
	float maxX = minX;
	float maxY = minY;
	//this process finds the smallest and largest points in the grid
	for (int i = 0; i < vertices.size(); i++)
	{
		if (minX > vertices[i].position.x)
		{
			minX = vertices[i].position.x;
		}
		if (maxX < vertices[i].position.x)
		{
			maxX = vertices[i].position.x;
		}
		if (minY > vertices[i].position.y)
		{
			minY = vertices[i].position.y;
		}
		if (maxY < vertices[i].position.y)
		{
			maxY = vertices[i].position.y;
		}
	}
	float* ret = (float*) malloc(3 * sizeof(float));
	
	ret[0] = std::max(maxX - minX, maxY - minY);
	ret[1] = 0.5 * (minX + maxX);
	ret[2] = 0.5 * (minY + maxY);
	
	return ret;
}
void Mesh::markVertices(const Vertex& v)
{
	//iterate over the shapes
	for (int j = 0; j < triangles.size(); j++)
	{
		Triangle *t = &triangles[j];
		
		//mark the points of the triangle for re-evaluation if they contain a vertex
		if (t->envelopes(v))
		{
			t->valid = false;
			edges.push_back(*t->e_ab);
			edges.push_back(*t->e_bc);
			edges.push_back(*t->e_ca);
		}
	}
}
void Mesh::checkEdges()
{
	//iterate over all the edges
	//run through all the edge permutations and see if the edges are equal
	for (int k = 0; k < edges.size(); k++)
	{
		Edge* e1 = &edges[k];
		std::cout << "Edge: (" << e1->start->index << ", " << e1->end->index << ")" << std::endl;
		
		for (int l = k + 1; l < edges.size(); l++)
		{
			Edge* e2 = &edges[l];
			std::cout << "\tEdge: (" << e2->start->index << ", " << e2->end->index << ") -> " ;
			if (e2->start->index == e2->end->index )
			{
				e2->valid = false;
				std::cout << "removed duplicate";
			}
			std::cout << std::endl;

			
			//if the edges lie too close to each other (roughly equal for this), they are not valid and need to be removed
			if (*e1 == *e2)
			{
				e1->valid = false;
				e2->valid = false;
				std::cout << "removed" << std::endl;
			}
		}
	}
}
void Mesh::GenerateTriangles(const Vertex& v)
{
	//for each remaining edge, let us construct a triangle with the points of an edge, and that point
	for (int m = 0; m < edges.size(); m++)
	{
		Edge* e = &edges[m];
		//be careful with the const-ness of the reference
		triangles.push_back( Triangle(e->start, e->end, const_cast<Vertex*>(&v)));
	}
}

void Mesh::index(void)
{
	//implementation of incremental delauney triangulation (ie the naive and easier to code version)
		
	//we already have the points inside the mesh, so let's find the super-triangle bounds
	float* vals;
	
	vals = bounds(vertices[0].position.x,  vertices[0].position.y);
	
	
	triangles.push_back( superTriangle(vals[0], vals[1], vals[2]) );
	const Vertex v1 = *triangles[0].e_ab->start;
	const Vertex v2 = *triangles[0].e_bc->start;
	const Vertex v3 = *triangles[0].e_ca->start;
	
	//now let's add the points one at a time and see if they make good triangles
	
	//iterate over the points
	for (int i = 0; i <vertices.size(); i++)
	{
		Vertex *v = &vertices[i];

		markVertices(*v);

		//delete any bad triangles by passing an iterator over the vector, and an anonymous function that checks the valid flag of each triangle
		triangles.erase(std::remove_if(begin(triangles), end(triangles), [](Triangle &t)
									   {
										   return !t.valid;
									   }
									   ), end(triangles));

		checkEdges();

		//another lambda delete, this time for the bad edges
		edges.erase(std::remove_if(begin(edges), end(edges), [](Edge &e)
								   {
									   return !e.valid;
								   }
								   ), end(edges));
		
		//regenerate the triangles, and repeat
		indexList();
		GenerateTriangles(*v);
		indexList();
	}
	//we should be set at this point, lets just check the triangles for validity and then construct the indices list
	
	//for all the triangles, check to see if the points of any one triangle are within another triangle, then remove them
	
	///////this function checks if any of the triangles are using the dummy points, and removing those triangles
	triangles.erase(std::remove_if(begin(triangles), end(triangles),
								   [v1, v2, v3](Triangle &t)
								   {
									   return t.contains(v1) || t.contains(v2) || t.contains(v3);
								   }
								   ), end(triangles));
	
	//add the edges back to the array of edges
	for (int i = 0; i < triangles.size(); i++)
	{
		Triangle *t = &triangles[i];
		edges.push_back(*t->e_ab);
		edges.push_back(*t->e_bc);
		edges.push_back(*t->e_ca);
	}
	
}
///////////////////////////////////////
//VERTEX implementations
/////////////////////////////////////////
Mesh::Vertex::Vertex()
{
	position = glm::vec3();
	normal = glm::vec3();
}
Mesh::Vertex::Vertex(glm::vec3 pos)
{
	position = pos;
	normal = glm::vec3();
}
Mesh::Vertex::Vertex(glm::vec3 pos, int i)
{
	position = pos;
	index = i;
}
Mesh::Vertex::Vertex(glm::vec3 pos, glm::vec3 norm)
{
	position = pos;
	normal = norm;
}
bool Mesh::Vertex::operator==(const Vertex &v)
{
	return (this->index == v.index);
}
bool Mesh::Vertex::equals(const Vertex &v)
{
	return (this->index == v.index);
}
float Mesh::Vertex::squareSum(void) const
{
	return position.x * position.x + position.y + position.y;
}
float Mesh::Vertex::squareDist(const Vertex &v) const
{
	float diffx = (this->position.x - v.position.x);
	float diffy = (this->position.x - v.position.x);

	return diffx * diffx + diffy * diffy;
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
float Mesh::Edge::slope()
{
	return (start->position.x - end->position.x) * (start->position.y - end->position.y);
}
void Mesh::Edge::flip()
{
	Vertex** tmp = &start;
	start = end;
	end = *tmp;
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
bool Mesh::Triangle::contains(const Vertex& v)
{
	return (	e_ab->start->equals(v)
			||	e_bc->start->equals(v)
			||	e_ca->start->equals(v));
}
bool Mesh::Triangle::envelopes(const Vertex &v)
{
	double ab = e_ab->start->squareSum();
	double bc = e_bc->start->squareSum();
	double ca = e_ca->start->squareSum();
	
	//TODO figure out what this math is up to!
	double Cx = (ab * (e_ca->start->position.y - e_bc->start->position.y)
				+ bc * (e_ab->start->position.y - e_ca->start->position.y)
				+ ca * (e_bc->start->position.y - e_ab->start->position.y))
				/
				(e_ab->start->position.x * (e_ca->start->position.y - e_bc->start->position.y)
				+ e_bc->start->position.x * (e_ab->start->position.y - e_ca->start->position.y)
				+ e_ca->start->position.x * (e_bc->start->position.y - e_ab->start->position.y));
	double Cy = (ab * (e_ca->start->position.x - e_bc->start->position.x)
				+ bc * (e_ab->start->position.x - e_ca->start->position.x)
				+ ca * (e_bc->start->position.x - e_ab->start->position.x))
				/
				(e_ab->start->position.y * (e_ca->start->position.x - e_bc->start->position.x)
				 + e_bc->start->position.y * (e_ab->start->position.x - e_ca->start->position.x)
				 + e_ca->start->position.y * (e_bc->start->position.x - e_ab->start->position.x));
	
	Vertex c( glm::vec3(0.5*Cx, 0.5*Cy, 0.0f) );
	double r = e_ab->start->squareDist(c);
	double d = v.squareDist(c);
	
	return (d <= r);
}














