//
//  renderer.h
//  COMP371-A1
//
//  Created by Mathew Plouffe on 2018-05-14.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef renderer_h
#define renderer_h

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "window.h"
#include "object.h"
#include "camera.h"
#include "texture.h"

class Renderer
{
private:
	GLuint modelFrameID, perspectiveFrameID, viewFrameID, shaderID;
	glm::mat4 viewMatrix, perspectiveMatrix, modelMatrix;
	int contextWidth, contextHeight;
public:
	std::vector<GLuint> vertexArrayBuffers;
	Renderer(GLuint shader);
	void init(Window::Window *w, Camera *c);
	void broadcastUniforms(void);
	void update(Window *w);
	void clear(void) const;
	
	void bind(Object *obj,GLenum DRAW_TYPE);
	GLuint bindTriangle(void);
	void bindTexture(Object *obj, Texture *tex);
	
	void drawArrays(GLuint vao, GLenum DRAW_TYPE, int shapeCount);
	void drawElements(GLuint vao, GLenum DRAW_TYPE, int indexCount);
	
	void transformViewMatrix(glm::mat4 transform);
	void transformModelMatrix(glm::mat4 transform);
	void transformProjectionMatrix(glm::mat4 transform);
private:
	void clearContext(void);
	void setContextSize(Window::Window *w);
	void setDepthTesting(void);
	
};

#endif /* renderer_h */
