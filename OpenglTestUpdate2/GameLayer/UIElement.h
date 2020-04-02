#pragma once
#include "../EngineLayer/Texture.h"
#include <string>

class UIElement
{
protected:
	struct UIVertex
	{
		float x;
		float y;
		float texX;
		float texY;
	};
	UIVertex vertexs[4];
	unsigned int Ids[6];
	GLuint VB;
	GLuint IB;
	std::string fileName;
	Texture* texture;
	float width;
	float height;
public:
	UIElement(std::string _fileName, float _x, float _y, float _width, float _height);
	~UIElement();
	bool Init();
	void Render();
};

class Button :public UIElement
{
private:
	void (*buttonFunc)();
public:
	Button(std::string _fileName, float _x, float _y, float _width, float _height) :
		UIElement(_fileName,_x,_y,_width,_height) {}
	bool IsClicked(float x, float y);
	void SetFunc(void (*func)());
};