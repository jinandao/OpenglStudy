#include "UIElement.h"
#include <glew.h>
#include <iostream>

UIElement::UIElement(std::string _fileName, float _x, float _y, float _width, float _height):
	fileName(_fileName), VB(0),width(_width),height(_height)
{
	texture = new Texture(fileName);
	vertexs[0] = { _x,_y,0,0 };
	vertexs[1] = { _x,_y+_height,0,1 };
	vertexs[2] = { _x+_width,_y + _height,1,1 };
	vertexs[3] = { _x+_width,_y,1,0 };

	Ids[0] = 0;
	Ids[1] = 1;
	Ids[2] = 2;
	Ids[3] = 0;
	Ids[4] = 2;
	Ids[5] = 3;
}

UIElement::~UIElement()
{
	if (texture != nullptr)
	{
		delete texture;
		texture = nullptr;
	}
}

bool UIElement::Init()
{
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Ids), Ids, GL_STATIC_DRAW);
	return true;
}

void UIElement::Render()
{

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	/*for (unsigned int i = 0; i < m_Entries.size(); i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->Bind(0);
		}

		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
	}*/

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 2,GL_FLOAT, GL_FALSE, sizeof(UIVertex),0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (const GLvoid*)8);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	texture->Bind(0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

bool Button::IsClicked(float x, float y)
{
	if ((x > vertexs[0].x) && (x - vertexs[0].x < width) && (y > vertexs[0].y) && (y-vertexs[0].y < height))
	{
		if (buttonFunc != nullptr)
		{
			buttonFunc();
		}
		return true;
	}
	return false;
}

void Button::SetFunc(void(*func)())
{
	buttonFunc = func;
}
