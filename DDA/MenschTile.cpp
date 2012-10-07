#include "MenschTile.h"
#include "MenschArgere.h"

MenschTile::MenschTile(int _x, int _y, QColor _color, MenschArgere * _game)
{
	x = _x;
	y = _y;
	color = _color;
	game = _game;
}

void MenschTile::Draw(QPainter * painter, int tickMillis)
{
	Draw(painter, color, tickMillis);
}

void  MenschTile::Draw(QPainter * painter, const QColor &tileColor, int tickMillis)
{
	float tileWidth = game->GetTileWidth() * 0.90f;
	float tileHeight = game->GetTileHeight() * 0.90f;
	float borderWidth = game->GetTileWidth() * 0.10f;
	float borderHeight = game->GetTileHeight() * 0.10f;
	
	painter->setBrush(QBrush(tileColor));
	painter->drawEllipse(borderWidth / 2.0f + x * tileWidth + x * borderWidth, painter->viewport().height() - (borderHeight + y * tileHeight + y * borderHeight) - tileHeight, tileWidth, tileHeight);
}

void MenschTile::DrawPlayer(QPainter * painter, const QColor & playerColor)
{
	float tileWidth = game->GetTileWidth() * 0.60f;
	float tileHeight = game->GetTileHeight() * 0.60f;
	float borderWidth = game->GetTileWidth() * 0.40f;
	float borderHeight = game->GetTileHeight() * 0.40f;
	
	painter->setBrush(QBrush(playerColor));
	painter->drawEllipse(borderWidth / 2.0f + x * tileWidth + x * borderWidth, painter->viewport().height() - (borderHeight + y * tileHeight + y * borderHeight) - painter->viewport().height() / 22.0f, tileWidth, tileHeight);
}

bool MenschTile::Contain(int pointX, int pointY)
{
	float tileWidth = game->GetTileWidth();
	float tileHeight = game->GetTileHeight();
	float boardHeight = game->GetBoardHeight();

	return (pointX > tileWidth * x && pointX < tileWidth * (x + 1) &&
		    boardHeight - pointY > (tileHeight * y) && boardHeight - pointY < (tileHeight * (y + 1)));
}