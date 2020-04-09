#include "Shader.h"
#include <iostream>
#include "../EngineLayer/OpenglTools.h"

ShaderProgram::ShaderProgram():shaderProgramID(0)
{
	shaderProgramID = glCreateProgram();

	if (shaderProgramID == 0)
	{
		std::cout << "Error creating shader program\n" << std::endl;
		exit(1);
	}
}

ShaderProgram::~ShaderProgram()
{
	if (shaderProgramID != 0)
	{
		glDeleteShader(shaderProgramID);
		shaderProgramID = 0;
	}
	/*for (auto ite = shaderObjList.begin(); ite != shaderObjList.end(); ite++)
	{
		glDeleteShader(*ite);
	}*/
}

//bool Shader::Init()
//{
//	if (shaderProgramID == 0)
//	{
//		CompileShader(shaderProgramID, vsFileName.c_str(), psFileName.c_str());
//	}
//	if (shaderProgramID != 0)
//	{
//		return true;
//	}
//	return false;
//}

void ShaderProgram::AddShader(std::string shaderFileName, GLenum shaderType)
{
	std::string shaderText;
	if (!ReadFile(shaderFileName.c_str(), shaderText))
	{
		exit(1);
	}

	GLuint ShaderObj = glCreateShader(shaderType);

	if (ShaderObj == 0) {
		std::cout << "Error creating shader type: " << shaderType << std::endl;
		exit(0);
	}

	const GLchar* p[1];
	//char* test=new char[shader.GetShaderText().size()+1];
	char* test = new char[shaderText.size() + 1];
	for (int i = 0; i < shaderText.size(); i++)
	{
		test[i] = shaderText[i];
	}
	test[shaderText.size()] = '\0';
	p[0] = test;

	GLint Lengths[1];
	Lengths[0] = strlen(shaderText.c_str());
	/*std::cout <<"test"<< test << std::endl;
	std::cout <<"length0:"<< Lengths[0] << std::endl;
	std::cout <<"length1:"<< strlen(p[0]) << std::endl;*/

	glShaderSource(ShaderObj, 1, p, Lengths);

	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cout << "Error compiling shader type: " << shaderType << " " << InfoLog << std::endl;
		exit(1);
	}
	glAttachShader(shaderProgramID, ShaderObj);
	shaderObjList.push_back(ShaderObj);
}

bool ShaderProgram::Finalize()
{
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	glLinkProgram(shaderProgramID);
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	glValidateProgram(shaderProgramID);
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	for (auto ite = shaderObjList.begin(); ite != shaderObjList.end(); ite++)
	{
		glDeleteShader(*ite);
	}
	return true;
}

GLint ShaderProgram::GetUniformLocation(const char* pUniformName)
{
	GLuint Location = glGetUniformLocation(shaderProgramID, pUniformName);

	if (Location == INVALID_UNIFORM_LOCATION) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
	}

	return Location;
}

GLint ShaderProgram::GetProgramParam(GLint param)
{
	GLint ret;
	glGetProgramiv(shaderProgramID, param, &ret);
	return GLint();
}
