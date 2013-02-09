#include <QGridLayout>
#include "BatchDiagrams.h"

BatchDiagrams::BatchDiagrams(BatchItem * _batchItem)
{
	batchItem = _batchItem;

	QGridLayout *gridLayout = new QGridLayout;
	QwtPlot* plot = new QwtPlot(this);
	histogram = new QwtPlotHistogram();
	histogram->attach(plot);
	SetData(batchItem->GetStatAsVector(BatchItem::STAT_TURN_NUMBER));
	gridLayout->addWidget(plot, 0, 0, 1, 1);
    setLayout(gridLayout);
}

BatchDiagrams::~BatchDiagrams(void)
{
}

void BatchDiagrams::SetData(vector<float> inputData, int collumnNumber)
{
	if(inputData.size() < 2)
		return;

	sort(inputData.begin(), inputData.end());
	float range = inputData.back() - inputData.front();
	float rangeBin = range / collumnNumber;
	
	float startBin = inputData[0];
	float endBin = inputData[0] + rangeBin;
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
		endBin = startBin + rangeBin;

        interval.setBorderFlags( QwtInterval::ExcludeMaximum );
        samples[i] = QwtIntervalSample( val, interval );
    }

    histogram->setData( new QwtIntervalSeriesData( samples ) );
}