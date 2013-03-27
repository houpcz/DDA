#include <QGridLayout>
#include "BatchDiagrams.h"

BatchDiagrams::BatchDiagrams(BatchItem * _batchItem, int batchID)
{
	batchItem = _batchItem;
	resize(500, 500);
	setWindowTitle(batchItem->Game()->GetGameName() + " " + QString::number(batchID + 1));

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
	gameStat->addItem("Justice");
	gameStat->addItem("Control");
	gameStat->addItem("Leader Time");
	gameStat->addItem("Player stat");

	playerStat = new QComboBox(this);
	playerStat->addItem("Turn number");
	playerStat->addItem("Winner");
	playerStat->addItem("Avg choises");
	playerStat->addItem("Min choises");
	playerStat->addItem("Max choises");
	playerStat->addItem("Justice");
	playerStat->addItem("LT");

	playerName = new QComboBox(this);
	for(int loop1 = 0; loop1 < batchItem->Game()->GetPlayerCount(); loop1++)
	{
		playerName->addItem(batchItem->Game()->GetPlayer(loop1)->GetAIName());
	}
	playerStat->setEnabled(false);
	playerName->setEnabled(false);
	connect(gameStat, SIGNAL(activated(int)), this, SLOT(ChangeGameStat(int)));
	connect(playerStat, SIGNAL(activated(int)), this, SLOT(ChangeGameStat(int)));
	connect(playerName, SIGNAL(activated(int)), this, SLOT(ChangeGameStat(int)));

	gridLayout->addWidget(gameStat, 0, 0, 1, 2);
	gridLayout->addWidget(playerStat, 1, 1, 1, 1);
	gridLayout->addWidget(playerName, 1, 0, 1, 1);
	gridLayout->addWidget(plot, 2, 0, 1, 2);
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
	int valueRange = 1;
	int lastValue = inputData[0];
	for(int loop1 = 1; loop1 < inputData.size(); loop1++)
	{
		if(lastValue != inputData[loop1])
		{
			valueRange++;
			lastValue = inputData[loop1];
		}
	}

	collumnNumber = min(collumnNumber, valueRange);
	QVector<QwtIntervalSample> samples( collumnNumber );

	if(valueRange == 1)
	{
		QwtInterval interval( inputData[0], inputData[0] + 0.00001);
		interval.setBorderFlags( QwtInterval::ExcludeMaximum );
		samples[0] = QwtIntervalSample( inputData.size(), interval );
		histogram->setAxes(100, 100);
		histogram->setData( new QwtIntervalSeriesData( samples ) );
		return;
	}

	float range = inputData.back() - inputData.front();
	float rangeBin = range / collumnNumber;
	
	float startBin = inputData[0];
	float endBin = inputData[0] + rangeBin;
	int lastInput = 0;
	
    for ( uint i = 0; i < collumnNumber; i++ )
	{
		int val = 0;
		while(lastInput < inputData.size() && inputData[lastInput] < endBin + 0.00001)
		{
			val++;
			lastInput++;
		}
        QwtInterval interval( startBin, endBin );
		startBin = endBin;
		endBin = startBin + rangeBin;

		interval.setBorderFlags( QwtInterval::ExcludeMaximum );
		samples[i] = QwtIntervalSample( val, interval );
    }

	histogram->setAxes(100, 100);
    histogram->setData( new QwtIntervalSeriesData( samples ) );
}

void BatchDiagrams::ChangeGameStat(int dontUseIt)
{
	UpdateDiagram();
}

void BatchDiagrams::UpdateDiagram()
{
	if(gameStat->currentIndex() == gameStat->count() - 1)
	{
		playerStat->setEnabled(true);
		playerName->setEnabled(true);
		SetHistogramData(batchItem->GetPlayerStatAsVector(playerStat->currentIndex(), playerName->currentIndex()), 15);
	} else {
		playerStat->setEnabled(false);
		playerName->setEnabled(false);
		SetHistogramData(batchItem->GetStatAsVector(gameStat->currentIndex()), 15);
	}

	plot->replot();
	plot->repaint();
}