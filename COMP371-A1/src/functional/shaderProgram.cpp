#include "shaderProgram.h"

//the shader class implementation file, provides implementations for all the functions in the shader header

ShaderProgram::ShaderProgram()
{
	//a completely empty shader object 
	//TODO specify some dinky default shaders here
}
ShaderProgram::ShaderProgram(char* vertexPath, char* fragmentPath)
{
	//I could have used strings here, but I like C_strings better
	vertPath = (char*) malloc (strlen(vertexPath) + 1);
	fragPath = (char*) malloc (strlen(fragmentPath) + 1);
	
	//ensure the strings are deep-copied
	strcpy(vertPath, vertexPath);
	strcpy(fragPath, fragmentPath);
}
ShaderProgram::ShaderProgram(const ShaderProgram& other)
{
	//I could have used strings here, see comment about C_strings
	this->vertPath = (char*) malloc (strlen(other.vertPath) + 1);
	this->fragPath = (char*) malloc (strlen(other.fragPath) + 1);
	
	//deep copy the c_strings
	strcpy(this->vertPath, vertPath);
	strcpy(this->fragPath, fragPath);
}
void ShaderProgram::init()
{
	//meat of the shader class, this function attempts to load, compile and and discard the shaders.
	//If successful, it frees the temporary c_strings it was using
		load();
		std::cout << "MESSAGE: Shaders loaded successfully" << std::endl;
		compile();
		std::cout << "MESSAGE: Shaders compiled successfully" << std::endl;
		attach();
		std::cout << "MESSAGE: Shaders attached successfully" << std::endl;
		discardShaders();
		std::cout << "MESSAGE: Shaders discarded after loading, deleting uncompiled data." << std::endl;
		free(vertPath);
		free(fragPath);
		free(vertCode);
		free(fragCode);
}
void ShaderProgram::load()
{
	//the shaders are loaded from the specified path, into c_strings
		vertCode = ShaderProgram::load(vertPath);
		fragCode = ShaderProgram::load(fragPath);
}
void ShaderProgram::compile()
{
	//the shaders are compiled as the appropriate type of shader
		vertID = compileShader(vertCode, GL_VERTEX_SHADER);
		fragID = compileShader(fragCode, GL_FRAGMENT_SHADER);
}
void ShaderProgram::attach(GLuint shaderID, GLuint progID)
{
	//the indvidual shader is linked to the larger shader program
	glAttachShader(progID, shaderID);
	glLinkProgram(progID);
	glValidateProgram(progID);	
	//glchar is gross
	GLchar log[512];
	int success;
	glGetProgramiv(progID, GL_LINK_STATUS, &success);
	
	//send out useful data if the shader does not link to the program
	if (!success)
	{
		glGetProgramInfoLog(progID, sizeof(log), NULL, log);
		std::stringstream err;
		err <<"Shaders could not be linked to the program\n";
		err << log;
		std::cout << err.str() << std::endl;
		throw GLException();
	}
}
void ShaderProgram::attach()
{
	//generate the programID (read location in GPU) for the entire shader, then attach the individual vertex and fragment shaders to it
	programID = glCreateProgram();
	ShaderProgram::attach(vertID, programID);
	ShaderProgram::attach(fragID, programID);
}
void ShaderProgram::discardShaders()
{
	//delete the shaders from the local memory, since they now reside on the GPU
	glDeleteShader(vertID);
	glDeleteShader(fragID);

}
void ShaderProgram::enable()
{
	//use the shader
	glUseProgram(programID);
}
void ShaderProgram::disable()
{
	//disuse the shader
	glDisable(programID);
}
char* ShaderProgram::load(char* path)
{
	//uses fseek to read the raw contents of the shader into the file
	FILE *fp = fopen(path, "r");
	
	//file could not be found
	if (fp == NULL)
	{
		std::stringstream err;
		err <<"File not found at the specififed path\nPath: ";
		err << path;
		std::cout << err.str() << std::endl;
		throw *new GLIOException();
	}
	
	//get the number of bytes to read in
	fseek(fp,0,SEEK_END);
	int bytes =(int) ftell(fp);
	char* code = (char *) malloc(bytes);
	
	//there is nothing in the shader, or it could not be read
	if (0 != fseek(fp,0,SEEK_SET) || bytes < 1)
	{
		std::stringstream err;
		err << "File at the specififed path could not be read, or is empty\nPath: ";
		err << path;
		std::cout << err.str() << std::endl;
		throw GLIOException();
	}
	
	//read the bytes in, and append a null terminator for safety
	fread(code,bytes,1,fp);
	code[bytes]= '\0';
	return code;
}
GLuint ShaderProgram::compileShader(char* code, GLenum SHADER_TYPE)
{
	//creates the SHADER_TYPE of shader and compiles the code
	GLuint frag = glCreateShader(SHADER_TYPE);
	glShaderSource(frag,1, &code, NULL);
	glCompileShader(frag);
	GLint success;
	GLchar log[512];
	glGetShaderiv( frag, GL_COMPILE_STATUS, &success );
	
	//shader could not be compiled, throw useful debug data
	if (!success)
	{
		glGetShaderInfoLog(frag,sizeof(log),NULL,log);
		std::stringstream err;
		err << "ERROR: shader could not be compiled by OpenGL. shader type: ";
		err << SHADER_TYPE << std::endl;
		err << code;
		std::cout << err.str() << std::endl;
		throw GLCompilationException();
	}
	return frag;		
}
