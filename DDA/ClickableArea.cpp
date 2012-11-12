#include "ClickableArea.h"


ClickableArea::ClickableArea(void)
{
}


ClickableArea::~ClickableArea(void)
{
}

void ClickableArea::SetSize(int width, int height)
{
	area.setWidth(width);
	area.setHeight(height);
}

void ClickableArea::MoveTo(int x, int y)
{
	area.moveTo(x, y);
}
