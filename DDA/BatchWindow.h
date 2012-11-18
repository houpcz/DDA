#ifndef _BATCH_WINDOW_H_
#define _BATCH_WINDOW_H_

#include <qwidget.h>
#include <QPushButton>
#include <QLCDNumber>
#include <QSpinBox>
#include <QComboBox>
#include <QTreeWidget>
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
	QPushButton *addBatch;
	QPushButton *removeBatch;
	QPushButton *setupBatch;
	QTreeWidget *listBatch;
	QComboBox * gameList;
	QSpinBox * batchSize;
	IGame *activeGame;
public:
	BatchWindow(QWidget *parent);
	~BatchWindow(void);
public slots:
	void StartBatch();
	void StopBatch();
	void RemoveTopItem();
	void AddItemToBatch();
};

#endif

