#include "Shader.h"
#include <iostream>
#include "OpenglTools.h"

void AddShader(GLuint ShaderProgram, Shader shader)
{
	GLuint ShaderObj = glCreateShader(shader.GetEnum());

	if (ShaderObj == 0) {
		std::cout << "Error creating shader type: " << shader.GetEnum() << std::endl;
		exit(0);
	}

	const GLchar* p[1];
	char* test=new char[shader.GetShaderText().size()+1];
	for (int i = 0; i < shader.GetShaderText().size(); i++)
	{
		test[i] = shader.GetShaderText()[i];
	}
	test[shader.GetShaderText().size()] = '\0';
	p[0] = test;
	
	GLint Lengths[1];
	Lengths[0] = strlen(shader.GetShaderText().c_str());
	//std::cout <<"length0:"<< Lengths[0] << std::endl;
	//std::cout <<"length1:"<< strlen(p[0]) << std::endl;

	glShaderSource(ShaderObj, 1, p, Lengths);
	
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cout << "Error compiling shader type: " << shader.GetEnum() <<" "<< InfoLog<< std::endl;
		exit(1);
	}
	glAttachShader(ShaderProgram, ShaderObj);
}

void CompileShader(GLuint& shaderProgram, const char* vsFileName,const char* fsFileName)
{
	shaderProgram = glCreateProgram();
	if (shaderProgram == 0)
	{
		std::cout << "Error creating shader program\n" << std::endl;
		exit(1);
	}
	std::string vs, fs;
	if (!ReadFile(vsFileName, vs))
	{
		exit(1);
	}
	if (!ReadFile(fsFileName, fs))
	{
		exit(1);
	}
	Shader vsShader(vs, GL_VERTEX_SHADER);
	Shader fsShader(fs, GL_FRAGMENT_SHADER);
	AddShader(shaderProgram, vsShader);
	AddShader(shaderProgram, fsShader);
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}
}
