#pragma once
#include <QWidget>
#include <QComboBox>
#include "BatchItem.h"
#include "Qwt\qwt_plot_histogram.h"
#include "Qwt\qwt_plot.h"

class BatchDiagrams :
	public QWidget
{
Q_OBJECT

private :
	BatchItem * batchItem;
	QComboBox * gameStat;

	QwtPlotHistogram * histogram;
	QwtPlot* plot;

	void SetHistogramData(vector<float> inputData, int collumnNumber = 10);
public:
	BatchDiagrams(BatchItem * _batchItem);
	~BatchDiagrams(void);

public slots:
	void ChangeGameStat(int newGameStat);
};

