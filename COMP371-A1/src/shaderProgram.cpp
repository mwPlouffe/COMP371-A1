#include "shaderProgram.h"

ShaderProgram::ShaderProgram()
{
	//a completely empty shader object 
	//TODO specify some dinky default shaders here
}
ShaderProgram::ShaderProgram(char* vertexPath, char* fragmentPath)
{
	//I could have used strings here, but oh well
	vertPath = (char*) malloc (strlen(vertexPath) + 1);
	fragPath = (char*) malloc (strlen(fragmentPath) + 1);
	
	strcpy(vertPath, vertexPath);
	strcpy(fragPath, fragmentPath);
}
ShaderProgram::ShaderProgram(const ShaderProgram& other)
{
	//I could have used strings here, but oh well
	this->vertPath = (char*) malloc (strlen(other.vertPath) + 1);
	this->fragPath = (char*) malloc (strlen(other.fragPath) + 1);
	
	strcpy(this->vertPath, vertPath);
	strcpy(this->fragPath, fragPath);
}
void ShaderProgram::init()
{
	try
	{
		load();
		std::cout << "MESSAGE: Shaders loaded successfully" << std::endl;
		compile();
		std::cout << "MESSAGE: Shaders compiled successfully" << std::endl;
		attach();
		std::cout << "MESSAGE: Shaders attached successfully" << std::endl;
		discardShaders();
		std::cout << "MESSAGE: Shaders discarded after loading" << std::endl;
	}
	catch(GLIOException &ex)
	{
		std::cout << "EXCEPTION: " << ex.what() << std::endl;
		throw;	
	}
	catch(GLCompilationException &ex)
	{
	std::cout << "EXCEPTION: "  << ex.what() << std::endl;
		throw;
	}
	catch(GLException &ex)
	{
		std::cout << "EXCEPTION: " << ex.what() << std::endl;
		throw;
	}
}
void ShaderProgram::load()
{
	try
	{
		vertCode = ShaderProgram::load(vertPath);
		//std::cout << vertCode << std::endl;
		fragCode = ShaderProgram::load(fragPath);
		//std::cout << fragCode << std::endl;
	}
	catch(GLIOException &ex)
	{
		throw;
	}
}
void ShaderProgram::compile()
{
	try
	{
		vertID = ShaderProgram::compileVertexShader(vertCode);
		fragID = ShaderProgram::compileFragmentShader(fragCode);
	}
	catch(GLCompilationException &ex)
	{
		throw;
	}
}
void ShaderProgram::attach(GLuint shaderID, GLuint progID)
{
	glAttachShader(progID, shaderID);
	glLinkProgram(progID);
	glValidateProgram(progID);	
	//glchar is gross
	GLchar log[512];
	int success;
	glGetProgramiv(progID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(progID, sizeof(log), NULL, log);
		char* err = (char*) malloc(100);
		strcat(err,"Shaders could not be linked to the program\n");
		strcat(err,log);
		throw GLException(err);
	}
}
void ShaderProgram::attach()
{
	programID = glCreateProgram();
	ShaderProgram::attach(vertID, programID);
	ShaderProgram::attach(fragID, programID);
}
void ShaderProgram::discardShaders()
{
	glDeleteShader(vertID);
	glDeleteShader(fragID);

}
char* ShaderProgram::load(char* path)
{
	FILE *fp = fopen(path, "r");
	if (fp == NULL)
	{	
		char* err = (char*) malloc(100);
		strcat(err,"File not found at the specififed path\nPath: ");
		strcat(err, path);
		throw GLIOException(err);
	}
	fseek(fp,0,SEEK_END);
	int bytes =(int) ftell(fp);
	char* code = (char *) malloc(bytes);
	if (0 != fseek(fp,0,SEEK_SET) || bytes < 1)
	{
		char* err = (char*) malloc(100);
		strcat(err, "File at the specififed path could not be read, or is empty\nPath: ");
		strcat(err, path);
		throw GLIOException(err);
	}
	fread(code,bytes,1,fp);
	code[bytes]= '\0';
	return code;
}
GLuint ShaderProgram::compileVertexShader(char* code)
{
	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert,1, &code, NULL);
	std::cout << code << std::endl;
	glCompileShader(vert);
	GLint success;
	GLchar log[512];
	glGetShaderiv( vert, GL_COMPILE_STATUS, &success );
	if (!success)
	{
		glGetShaderInfoLog(vert,sizeof(log),NULL,log);
		char* err = (char*) malloc(100);
                strcat(err,"Vertex shader could not be compiled by OpenGL\n");
		strcat(err,log);
		strcat(err,code);
		throw GLCompilationException(err);
	}
	return vert;		
}
GLuint ShaderProgram::compileFragmentShader(char* code)
{
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag,1, &code, NULL);
	std::cout << code << std::endl;
	glCompileShader(frag);
	GLint success;
	GLchar log[512];
	glGetShaderiv( frag, GL_COMPILE_STATUS, &success );
	
	if (!success)
	{
		glGetShaderInfoLog(frag,sizeof(log),NULL,log);
		char* err = (char*) malloc(100);
                strcat(err,"Fragment Shader could not be linked to the program\n");
		strcat(err,log);
		strcat(err,code);
		throw GLCompilationException(err);
	}
	return frag;		
}
