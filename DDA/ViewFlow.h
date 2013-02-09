#pragma once
#include <QWidget>
#include "Qwt\qwt_plot_item.h"
#include "IGame.h"

class ViewFlow :
	public QWidget
{
Q_OBJECT

private :
	double ** xVal;
	double ** yVal;
	IGame * game;
	QwtPlot *plot;

public:
	ViewFlow(IGame * _game);
	~ViewFlow(void);
public slots:
	void ShowItem( QwtPlotItem *item, bool on );
};

