#ifndef _BATCH_WINDOW_H_
#define _BATCH_WINDOW_H_

#include <qwidget.h>
#include <QPushButton>
#include <QLCDNumber>
#include "BatchThread.h"
#include "IGame.h"

class BatchWindow : public QWidget
{
    Q_OBJECT

private :
	BatchThread *batchThread;
	QLCDNumber *gameIDNumber;
	QPushButton *startButton;
	QPushButton *stopButton;
	IGame *activeGame;
public:
	BatchWindow(QWidget *parent);
	~BatchWindow(void);
public slots:
	void StartBatch();
	void StopBatch();
};

#endif

