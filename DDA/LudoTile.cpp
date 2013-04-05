#include "LudoTile.h"
#include "Ludo.h"

LudoTile::LudoTile(int _x, int _y, QColor _color, Ludo * _game)
{
	x = _x;
	y = _y;
	color = _color;
	game = _game;
}

void LudoTile::Draw(QPainter * painter, int tickMillis)
{
	Draw(painter, color, tickMillis);
}

void  LudoTile::Draw(QPainter * painter, const QColor &tileColor, int tickMillis)
{
	float tileWidth = game->GetTileWidth() * 0.90f;
	float tileHeight = game->GetTileHeight() * 0.90f;
	float borderWidth = game->GetTileWidth() * 0.10f;
	float borderHeight = game->GetTileHeight() * 0.10f;
	
	painter->setBrush(QBrush(tileColor));
	painter->drawEllipse(game->BoardX() + borderWidth / 2.0f + x * tileWidth + x * borderWidth, game->BoardY() + painter->viewport().height() - (borderHeight + y * tileHeight + y * borderHeight) - tileHeight, tileWidth, tileHeight);
}

void LudoTile::DrawPlayer(QPainter * painter, const QColor & playerColor)
{
	float tileWidth = game->GetTileWidth() * 0.60f;
	float tileHeight = game->GetTileHeight() * 0.60f;
	float borderWidth = game->GetTileWidth() * 0.40f;
	float borderHeight = game->GetTileHeight() * 0.40f;
	
	int bX = game->BoardX();
	int bY = game->BoardY();
	QRadialGradient radialGradient(bX + borderWidth / 2.0f + x * tileWidth + x * borderWidth + tileWidth / 2.0f, bY + painter->viewport().height() - (borderHeight + y * tileHeight + y * borderHeight) - painter->viewport().height() / 22.0f + tileWidth / 2.0, tileWidth, tileWidth, tileHeight);
    radialGradient.setColorAt(0.0, Qt::white);
	radialGradient.setColorAt(0.5, playerColor);
    radialGradient.setColorAt(1.0, Qt::black);
	QBrush brush(radialGradient);
	painter->setBrush(brush);
	painter->drawEllipse(bX + borderWidth / 2.0f + x * tileWidth + x * borderWidth, bY + painter->viewport().height() - (borderHeight + y * tileHeight + y * borderHeight) - painter->viewport().height() / 22.0f, tileWidth, tileHeight);
}

bool LudoTile::Contain(int pointX, int pointY)
{
	float tileWidth = game->GetTileWidth();
	float tileHeight = game->GetTileHeight();
	float boardHeight = game->GetBoardHeight();
	int bX = game->BoardX();
	int bY = game->BoardY();

	return (pointX > bX + tileWidth * x && pointX < bX + tileWidth * (x + 1) &&
		    boardHeight - pointY > (tileHeight * y) + bY && boardHeight - pointY < (tileHeight * (y + 1) + bY));
}