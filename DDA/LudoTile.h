#pragma once

#include "qcolor.h"
#include "qpainter.h"

class Ludo;

class LudoTile
{
	int x;
	int y;
	QColor color;
	Ludo * game;
public:
	LudoTile(int _x, int _y, QColor _color, Ludo * _game);
	bool Contain(int pointX, int pointY);
	void Draw(QPainter * painter, const QColor &tileColor, int tickMillis = 0);
	void Draw(QPainter * painter, int tickMillis = 0);
	void DrawPlayer(QPainter * painter, const QColor & playerColor);
	void SetColor(QColor _color) { color = _color; };
};

