#pragma once

#include <QRect>

class ClickableArea
{
public :
	static const int EXPEDITION = 8;
	static const int DISCARD = 13;
	static const int DECK = 18;

private :
	QRect area;
	int cardID;
	bool active;

public :
	ClickableArea(void);
	~ClickableArea(void);
	int X() { return area.x(); }
	int Y() { return area.y(); }
	int CardID() { return cardID; }
	QRect Area() { return area; }
	bool Active() { return active; }
	void SetActive(bool _active) { active = _active; }
	void SetCardID(int _cardID) { cardID = _cardID; }
	void SetHeight(int height) { area.setHeight(height); }
	void SetSize(int width, int height);
	void MoveTo(int x, int y);
};

