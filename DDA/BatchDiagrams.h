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
	QComboBox * playerStat;
	QComboBox * playerName;
	QwtPlotHistogram * histogram;
	QwtPlot* plot;

	void SetHistogramData(vector<float> inputData, int collumnNumber = 10);
	void UpdateDiagram();
public:
	BatchDiagrams(BatchItem * _batchItem, int batchID);
	~BatchDiagrams(void);

public slots:
	void ChangeGameStat(int dontUseIt);
};

