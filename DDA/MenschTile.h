#pragma once

#include "qcolor.h"
#include "qpainter.h"

class MenschArgere;

class MenschTile
{
	int x;
	int y;
	QColor color;
	MenschArgere * game;
public:
	MenschTile(int _x, int _y, QColor _color, MenschArgere * _game);
	bool Contain(int pointX, int pointY);
	void Draw(QPainter * painter, const QColor &tileColor, int tickMillis = 0);
	void Draw(QPainter * painter, int tickMillis = 0);
	void DrawPlayer(QPainter * painter, const QColor & playerColor);
};

