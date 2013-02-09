#pragma once
#include <QWidget>
#include "BatchItem.h"
#include "Qwt\qwt_plot_histogram.h"
#include "Qwt\qwt_plot.h"

class BatchDiagrams :
	public QWidget
{
Q_OBJECT

private :
	BatchItem * batchItem;
	QwtPlotHistogram * histogram;

	void SetData(vector<float> inputData, int collumnNumber = 10);
public:
	BatchDiagrams(BatchItem * _batchItem);
	~BatchDiagrams(void);
};

