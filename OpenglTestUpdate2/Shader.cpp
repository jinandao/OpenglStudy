#include "Shader.h"
#include <iostream>

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
	//std::cout <<"test1 "<< shader.GetShaderText().c_str() << std::endl;
	//std::cout <<"test2 "<< test << std::endl;
	//std::cout << "test3 " << p[0] << std::endl;
	
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
