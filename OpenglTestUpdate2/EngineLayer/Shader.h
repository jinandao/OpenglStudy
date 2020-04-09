#pragma once
#include <glew.h>
#include <freeglut.h>
#include <string>
#include <iostream>
#include <list>

class ShaderProgram
{
private:
	GLuint shaderProgramID;
	//std::string vsFileName;
	//std::string psFileName;
	std::list<GLuint> shaderObjList;
	//std::string shaderText;

public:
	/*Shader(std::string _vsfileName,std::string _psfileName):vsFileName(_vsfileName),psFileName(_psfileName), shaderProgramID(0)
	{}*/
	ShaderProgram();
	~ShaderProgram();
	
	void AddShader(std::string shaderFileName, GLenum shaderType);
	bool Finalize();
	
	GLuint ShaderID()const { return shaderProgramID; }
	void Bind() { glUseProgram(shaderProgramID); }

	GLint GetUniformLocation(const char* pUniformName);
	GLint GetProgramParam(GLint param);
};

//void AddShader(GLuint ShaderProgram, std::string& shaderText, GLenum shaderType);
//static void CompileShader(GLuint& shaderProgram, const char* vsFileName, const char* fsFileName);




