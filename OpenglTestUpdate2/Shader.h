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
	{}	
	GLenum GetEnum() const { return shaderType; }
	std::string GetShaderText() const{ return shaderText; }
};

void AddShader(GLuint ShaderProgram, Shader shader);

void CompileShader(GLuint& shaderProgram,const char* vsFileName, const char* fsFileName);




