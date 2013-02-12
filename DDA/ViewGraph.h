#pragma once

#include <QWidget>
#include "Qwt\qwt_plot_item.h"
#include "IGame.h"
#include "IGraph.h"

class ViewGraph :
	public QWidget
{
public :
Q_OBJECT

private :
	double ** xVal;
	double ** yVal;
	IGraph * graphKind;
	IGame * game;
	QwtPlot *plot;

public:
	ViewGraph(IGame * _game, IGraph  * _graphKind);
	~ViewGraph(void);
public slots:
	void ShowItem( QwtPlotItem *item, bool on );
};

