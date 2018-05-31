#include "renderer.h"

//this class renders by Object or by vao
//it is the "driving force" behind this assignment

Renderer::Renderer(GLuint shader)
{
	shaderID = shader;
}
void Renderer::init(Window::Window *w, Camera *c)
{
	//set the view, model, and projection matrices
	setContextSize(w);
	//glEnable(GL_CULL_FACE);
	viewMatrix = c->init();
	
	
	perspectiveMatrix = glm::perspective(45.0f, (GLfloat)contextWidth / (GLfloat)contextHeight, 0.01f, 500.0f);
	
	modelMatrix = glm::scale(glm::mat4(), glm::vec3(1.0f));
	
	//find the corresponding uniforms in the shaders
	modelFrameID = glGetUniformLocation(shaderID, "model_matrix");
	viewFrameID = glGetUniformLocation(shaderID, "view_matrix");
	perspectiveFrameID = glGetUniformLocation(shaderID, "projection_matrix");
	//setDepthTesting();
}
void Renderer::broadcastUniforms(void)
{
	//broadcast (read re-load to the GPU) the new data to the shaders
	glUniformMatrix4fv(modelFrameID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(perspectiveFrameID, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));
	glUniformMatrix4fv(viewFrameID,1,GL_FALSE,glm::value_ptr(viewMatrix));
}
void Renderer::update(Window *w)
{
	//render the contents stored in the buffer
	broadcastUniforms();
	//glCullFace(GL_BACK);
	glfwSwapBuffers(w->glWindow());
	
	//check if there are any events to process
	glfwPollEvents();
}
void Renderer::setContextSize(Window::Window *w)
{
	//return the size of the frame buffer
	glfwGetFramebufferSize(w->glWindow(), &contextWidth, &contextHeight);
}
void Renderer::clear() const
{
	//clears the colour buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
void Renderer::setDepthTesting(void)
{
	//enables depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}
void Renderer::clearContext()
{
	//unbinds any currently bound VAO
	glBindVertexArray(0);
}
GLuint Renderer::bindTriangle(void)
{
	//the debug triangles
	GLuint vao, vbo, ibo;
	
	//hard-coded vertices to create a square from 3 triangles
	GLfloat vertices[] = {
		0.0f, 0.0f, 0.0f,  // Top
		  // Bottom Right
		0.0f, 1.0f, 0.0f,  // Bottom Left
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};
	
	//the indices needed to properly render that shader
	GLuint indices[] =
	{
		0,1,2,
		2,3,0
	};
	
	//create vao on the GPU and make it the working container
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	
	//create a vbo container, and add it to the vao
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	//store the vertex data in the vbo (the what to draw of the shape)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	
	//provide a location reference that is used by the shaders to locate the vbo inside the vao,
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	//generate an index buffer, and make it current
	glGenBuffers(1,&ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	
	//provide the data to the buffer (it stores how to draw the shape)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//clear the focus of the OGL state machine
	clearContext();
	return vao;
}
void Renderer::drawArrays(GLuint vao, GLenum DRAW_TYPE, int vertexCount)
{
	//draw using the vertex ordering
	glBindVertexArray(vao);
	glDrawArrays(DRAW_TYPE, 0, vertexCount);
	clearContext();
}
void Renderer::drawElements(GLuint vao, GLenum DRAW_TYPE, int indexCount)
{
	//draw using an ibo
	glBindVertexArray(vao);
	glDrawElements(DRAW_TYPE, indexCount, GL_UNSIGNED_INT, nullptr);
	clearContext();
}
void Renderer::bind(Object *obj,GLenum DRAW_TYPE)
{
	//sends the data from a renderable object the the GPU
	//process is identical to the debug triangle process, with the exception of colour
	GLuint vao, vbo, ibo, cbo;
	
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, obj->vertexSize() * sizeof(GLfloat), &(obj->vertexList())[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	//if the object stores data about colour, create a vbo to store that data
	if (obj->colourSize() > 0)
	{
		glGenBuffers(1,&cbo);
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		glBufferData(GL_ARRAY_BUFFER, obj->colourSize() * sizeof(GLfloat), &(obj->colourList())[0], GL_STATIC_DRAW);
		
		//this location must be different than the one used for the vertices
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
	}
	
	glGenBuffers(1,&ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->indexSize() * sizeof(GLuint), &(obj->pointIndex())[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);
	//instead of returning the vao, the object stores the vao its data is bound to
	obj->vao = vao;
}
void Renderer::bindTexture(Object *obj, Texture *tex)
{
	//generate a vbo for the texture coordinates
	//TODO
}
void Renderer::transformViewMatrix(glm::mat4 transform)
{
	//transform the view matrix
	viewMatrix = transform * viewMatrix;
}
void Renderer::transformModelMatrix(glm::mat4 transform)
{
	//transform the model matrix
	modelMatrix = transform * modelMatrix;
}
void Renderer::transformProjectionMatrix(glm::mat4 transform)
{
	//transform the perspective matrix
	perspectiveMatrix = transform * perspectiveMatrix;
}





