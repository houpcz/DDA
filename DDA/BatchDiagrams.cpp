#include <QGridLayout>
#include "BatchDiagrams.h"

BatchDiagrams::BatchDiagrams(BatchItem * _batchItem)
{
	batchItem = _batchItem;
	resize(500, 500);

	QGridLayout *gridLayout = new QGridLayout;
	plot = new QwtPlot(this);

	histogram = new QwtPlotHistogram();
	histogram->attach(plot);
	QwtColumnSymbol *symbol = new QwtColumnSymbol( QwtColumnSymbol::Box );
    symbol->setFrameStyle( QwtColumnSymbol::Raised );
    symbol->setLineWidth( 1 );
	symbol->setPalette( QPalette( Qt::blue ) );
    histogram->setSymbol( symbol );
	plot->setAxisTitle( QwtPlot::yLeft, "Number of Games" );
	SetHistogramData(batchItem->GetStatAsVector(0));

	gameStat = new QComboBox(this);
	gameStat->addItem("Turn number");
	gameStat->addItem("Player 1 winner");
	gameStat->addItem("Leader switches");
	gameStat->addItem("Avg score diff");
	gameStat->addItem("End score diff");
	connect(gameStat, SIGNAL(activated(int)), this, SLOT(ChangeGameStat(int)));

	gridLayout->addWidget(gameStat, 0, 0, 1, 1);
	gridLayout->addWidget(plot, 1, 0, 1, 1);
    setLayout(gridLayout);
}

BatchDiagrams::~BatchDiagrams(void)
{
}

void BatchDiagrams::SetHistogramData(vector<float> inputData, int collumnNumber)
{
	if(inputData.size() < 2)
		return;

	sort(inputData.begin(), inputData.end());
	float range = inputData.back() - inputData.front();
	float rangeBin = range / collumnNumber;
	
	float startBin = inputData[0];
	float endBin = inputData[0] + rangeBin + 0.0001f;
	int lastInput = 0;
	QVector<QwtIntervalSample> samples( collumnNumber );
    for ( uint i = 0; i < collumnNumber; i++ )
	{
		int val = 0;
		while(lastInput < inputData.size() && inputData[lastInput] < endBin)
		{
			val++;
			lastInput++;
		}
        QwtInterval interval( startBin, endBin );
		startBin = endBin;
		endBin = startBin + rangeBin + 0.0001f;

        interval.setBorderFlags( QwtInterval::ExcludeMaximum );
        samples[i] = QwtIntervalSample( val, interval );
    }

	histogram->setAxes(100, 100);
    histogram->setData( new QwtIntervalSeriesData( samples ) );
}

void BatchDiagrams::ChangeGameStat(int newGameStat)
{
	SetHistogramData(batchItem->GetStatAsVector(newGameStat), 15);
	//plot->setAxisTitle( QwtPlot::xBottom, gameStat->currentText() );
	plot->replot();
	plot->repaint();
}