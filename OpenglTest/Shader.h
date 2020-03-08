#pragma once
#include <glew.h>
#include <freeglut.h>
#include <string>
#include <iostream>

class Shader
{
private:
	GLenum shaderType;
	std::string shaderText;
public:
	Shader(std::string _shaderText, GLenum _shaderType) :shaderType(_shaderType), shaderText(_shaderText)
	{
		//std::cout << _shaderText << std::endl;
		//std::cout << shaderText << std::endl;
		//std::cout << shaderText.c_str() << std::endl;
	}	
	GLenum GetEnum() const { return shaderType; }
	std::string GetShaderText() const{ return shaderText; }
};

void AddShader(GLuint ShaderProgram, Shader shader);




