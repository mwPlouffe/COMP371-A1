#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H


#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLException.h"
#include "GLIOException.h"
#include "GLCompilationException.h"
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
class ShaderProgram
{
	//badness occurring here from duplication - refactor at some point	

	//private members
	private:
	char* vertPath;
	char* vertCode;
	char* fragPath;
	char* fragCode;
	GLuint vertID;
	GLuint fragID;
	public:
	GLuint programID;

	//constructors
	public:
	ShaderProgram();
	ShaderProgram(char* vertexPath, char* fragmentPath);
	ShaderProgram(const ShaderProgram& other);
	~ShaderProgram();

	//outward-facing functionalities
	void init();
	void compile();
	void load();
	void attach();
	void discardShaders();
	void enable();
	void disable();
	inline GLuint program()
	{ 
	return (programID != NULL) ? programID : GL_FALSE;
	};
	//helpers and utilities
	private:
	GLuint compileShader(char* code, GLenum SHADER_TYPE);
	static char* load(char* path);
	static void attach(GLuint shaderID, GLuint progID);
};
#endif 
