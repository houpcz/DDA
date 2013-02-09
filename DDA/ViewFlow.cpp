#include "ViewFlow.h"
#include <QGridLayout>
#include "Qwt\qwt_plot.h"
#include "Qwt\qwt_plot_curve.h"
#include "Qwt\qwt_legend_item.h"

ViewFlow::ViewFlow(IGame * _game)
{
	game = _game;

	resize(500, 500);
	setWindowTitle("Flow View");

	QGridLayout *gridLayout = new QGridLayout;
	plot = new QwtPlot(this);
	QwtPlotCurve ** graph = new QwtPlotCurve*[game->GetPlayerCount() - 1];
	xVal = new double*[game->GetPlayerCount() - 1];
	yVal = new double*[game->GetPlayerCount() - 1];
	vector<IGameState *> gameState = game->GetAllGameState();
	
	QwtLegend *legend = new QwtLegend;
	legend->setItemMode( QwtLegend::CheckableItem );
	plot->insertLegend( legend, QwtPlot::RightLegend );
	bool ok = connect( plot, SIGNAL( legendChecked( QwtPlotItem *, bool ) ), this, SLOT( ShowItem( QwtPlotItem *, bool ) ) );
	plot->replot();

	for(int loop1 = 0; loop1 < game->GetPlayerCount() - 1; loop1++)
	{
		xVal[loop1] = new double[gameState.size()];
		yVal[loop1] = new double[gameState.size()];
		QColor color = QColor((Qt::GlobalColor) ((loop1 + 7) % 16)); 
		IPlayer * player = game->GetPlayer(loop1 + 1);
		graph[loop1] = new QwtPlotCurve(player->GetAIName() + " " + QString::number(player->Level()));
		for(int loop2 = 0; loop2 < gameState.size(); loop2++)
		{
			xVal[loop1][loop2] = loop2;
			yVal[loop1][loop2] = gameState[loop2]->GetPlayerScore(loop1 + 1, 0);
		}
		graph[loop1]->setRawSamples(xVal[loop1], yVal[loop1], gameState.size());
		graph[loop1]->setPen(color);
		graph[loop1]->attach(plot);
	}
	gridLayout->addWidget(plot, 0, 0, 1, 1);
	setLayout(gridLayout);
}

void ViewFlow::ShowItem( QwtPlotItem *item, bool on )
{
    item->setVisible( !on );
	plot->replot();
	plot->repaint();
}


ViewFlow::~ViewFlow(void)
{
	for(int loop1 = 0; loop1 < game->GetPlayerCount() - 1; loop1++)
	{
		delete [] xVal[loop1];
		delete [] yVal[loop1];
	}
	delete [] xVal;
	delete [] yVal;
}
