#include "RenderItem.h"

void Destroy(RenderItem* item)
{
	if (item != nullptr)
	{
		delete item;
		item = nullptr;
	}
}