#pragma once
#include <string>
#include <glew.h>

using namespace std;

class Texture
{
public:
	unsigned int rendererID;
	std::string filePath;
	unsigned char* localBuffer;
	int width, height, bPP;

public:
	Texture(unsigned int id, const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
};

class CubemapTexture
{
private:
	string m_fileNames[6];
	GLuint m_textureObj;

public:
	CubemapTexture(const string& Directory,
		const string& PosXFilename,
		const string& NegXFilename,
		const string& PosYFilename,
		const string& NegYFilename,
		const string& PosZFilename,
		const string& NegZFilename);

	~CubemapTexture();

	bool Load();
	void Bind(GLenum TextureUnit);
};