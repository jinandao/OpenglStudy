#pragma once
#include <glew.h>
#include <freeglut.h>
#include <string>
#include <iostream>

class Shader
{
private:
	GLuint shaderID;
	std::string vsFileName;
	std::string psFileName;
	//std::string shaderText;

public:
	Shader(std::string _vsfileName,std::string _psfileName):vsFileName(_vsfileName),psFileName(_psfileName),shaderID(0)
	{}
	
	~Shader();
	bool Init();	
	GLuint ShaderID()const { return shaderID; }
};

static void AddShader(GLuint ShaderProgram, std::string& shaderText, GLenum shaderType);

static void CompileShader(GLuint& shaderProgram, const char* vsFileName, const char* fsFileName);




