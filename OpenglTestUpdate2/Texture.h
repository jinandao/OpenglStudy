#pragma once
#include <string>

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

